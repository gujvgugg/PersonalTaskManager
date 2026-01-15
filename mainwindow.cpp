#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "taskdialog.h"
#include "datamanager.h"
#include "categorydialog.h"
#include "reminderworker.h"
#include "networkmanager.h"
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlRelationalDelegate>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>
#include <QPrinter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>

// Custom Proxy Model for multi-column filtering
class TaskProxyModel : public QSortFilterProxyModel
{
public:
    TaskProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}

    int filterPriority = -1; // -1 means all
    int filterStatus = -1;   // -1 means all, 0: Pending, 1: Done
    int filterCategory = -1; // Unused in this version
    QString filterCategoryName; // Empty means all

    void setPriorityFilter(int priority) {
        filterPriority = priority;
        invalidateFilter();
    }

    void setStatusFilter(int status) {
        filterStatus = status;
        invalidateFilter();
    }

    void setCategoryNameFilter(const QString &name) {
        filterCategoryName = name;
        invalidateFilter();
    }

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
        // Get source model
        QAbstractItemModel *model = sourceModel();

        // Check Priority (Column 2)
        if (filterPriority != -1) {
            QModelIndex priorityIndex = model->index(source_row, 2, source_parent);
            int priority = model->data(priorityIndex).toInt();
            if (priority != filterPriority) return false;
        }

        // Check Status (Column 3)
        if (filterStatus != -1) {
            QModelIndex doneIndex = model->index(source_row, 3, source_parent);
            int done = model->data(doneIndex).toInt();
            if (done != filterStatus) return false;
        }

        // Check Category (Column 6)
        if (!filterCategoryName.isEmpty()) {
            QModelIndex categoryIndex = model->index(source_row, 6, source_parent);
            QString category = model->data(categoryIndex).toString();
            if (category != filterCategoryName) return false;
        }

        return true;
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_reminderWorker(nullptr)
    , m_workerThread(nullptr)
    , m_networkManager(nullptr)
{
    ui->setupUi(this);

    // Initialize DataManager
    if (!DataManager::instance().openDatabase()) {
        QMessageBox::critical(this, tr("错误"), tr("无法打开数据库"));
    }

    // Setup Filters
    ui->priorityFilter->clear();
    ui->priorityFilter->addItem(tr("所有优先级"), -1);
    ui->priorityFilter->addItem(tr("低"), 0);
    ui->priorityFilter->addItem(tr("中"), 1);
    ui->priorityFilter->addItem(tr("高"), 2);

    ui->statusFilter->clear();
    ui->statusFilter->addItem(tr("所有状态"), -1);
    ui->statusFilter->addItem(tr("未完成"), 0);
    ui->statusFilter->addItem(tr("已完成"), 1);

    // Connect Signals
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(ui->editButton, &QPushButton::clicked, this, &MainWindow::editTask);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::deleteTask);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportReport);
    connect(ui->categoryButton, &QPushButton::clicked, this, &MainWindow::manageCategories);
    connect(ui->syncButton, &QPushButton::clicked, this, &MainWindow::checkCloudSync);

    connect(ui->priorityFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFilterChanged);
    connect(ui->statusFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFilterChanged);
    connect(ui->categoryFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFilterChanged);

    setupModel();
    setupReminders();
    setupNetwork();

    // Add status label
    m_statusLabel = new QLabel(this);
    ui->statusbar->addPermanentWidget(m_statusLabel);
    m_statusLabel->setText(tr("就绪"));
}

MainWindow::~MainWindow()
{
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
    delete ui;
}

void MainWindow::setupModel()
{
    m_model = new QSqlRelationalTableModel(this, DataManager::instance().getDatabase());
    m_model->setTable("tasks");
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->setJoinMode(QSqlRelationalTableModel::LeftJoin);

    // Set Relation: column 6 (category_id) -> categories.id, show name
    m_model->setRelation(6, QSqlRelation("categories", "id", "name"));

    m_model->select();

    // Set headers
    m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("标题"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("优先级"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("完成状态"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("截止时间"));
    m_model->setHeaderData(5, Qt::Horizontal, tr("详情"));
    m_model->setHeaderData(6, Qt::Horizontal, tr("分类"));

    TaskProxyModel *proxy = new TaskProxyModel(this);
    proxy->setSourceModel(m_model);
    m_proxyModel = proxy;

    ui->tableView->setModel(m_proxyModel);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->hideColumn(0); // Hide ID

    refreshCategories();
}

void MainWindow::setupReminders()
{
    m_workerThread = new QThread(this);
    m_reminderWorker = new ReminderWorker(DataManager::instance().getDatabasePath());
    m_reminderWorker->moveToThread(m_workerThread);

    connect(m_workerThread, &QThread::started, m_reminderWorker, &ReminderWorker::startWork);
    connect(m_workerThread, &QThread::finished, m_reminderWorker, &ReminderWorker::deleteLater);
    connect(m_reminderWorker, &ReminderWorker::reminderTriggered, this, &MainWindow::onReminderTriggered);

    m_workerThread->start();
}

void MainWindow::setupNetwork()
{
    m_networkManager = new NetworkManager(this);
    connect(m_networkManager, &NetworkManager::connectionStatus, this, &MainWindow::onNetworkStatus);
}

void MainWindow::refreshCategories()
{
    ui->categoryFilter->blockSignals(true);
    ui->categoryFilter->clear();
    ui->categoryFilter->addItem(tr("所有分类"), -1);

    QSqlQuery query("SELECT id, name FROM categories", DataManager::instance().getDatabase());
    while (query.next()) {
        ui->categoryFilter->addItem(query.value(1).toString(), query.value(0).toInt());
    }
    ui->categoryFilter->blockSignals(false);
}

void MainWindow::manageCategories()
{
    CategoryDialog dialog(this);
    dialog.exec();
    // Refresh model and filter
    m_model->select(); // Re-select to update relations
    refreshCategories();
}

void MainWindow::addTask()
{
    TaskDialog dialog(this);

    // Populate categories
    QMap<int, QString> categories;
    QSqlQuery query("SELECT id, name FROM categories", DataManager::instance().getDatabase());
    while (query.next()) {
        categories.insert(query.value(0).toInt(), query.value(1).toString());
    }
    dialog.populateCategories(categories);

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query(DataManager::instance().getDatabase());
        query.prepare("INSERT INTO tasks (title, priority, done, deadline, detail, category_id) "
                      "VALUES (?, ?, ?, ?, ?, ?)");
        query.addBindValue(dialog.getTitle());
        query.addBindValue(dialog.getPriority());
        query.addBindValue(0);
        query.addBindValue(dialog.getDeadline().toString("yyyy-MM-dd HH:mm"));
        query.addBindValue(dialog.getDetail());
        query.addBindValue(dialog.getCategoryId());

        if (query.exec()) {
            m_model->select(); // Refresh model to show new data and resolve relations
            
            // Force proxy model invalidation just in case
            if (m_proxyModel) m_proxyModel->invalidate();
            
            qDebug() << "✅ Task added successfully, Category ID:" << dialog.getCategoryId();

            // Reset filters to ensure new task is visible
            ui->priorityFilter->setCurrentIndex(0);
            ui->statusFilter->setCurrentIndex(0);
            ui->categoryFilter->setCurrentIndex(0);
        } else {
            QMessageBox::critical(this, tr("错误"), tr("保存失败: ") + query.lastError().text());
        }
    }
}

void MainWindow::editTask()
{
    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    if (!index.isValid()) return;

    // Map to source model
    QModelIndex sourceIndex = m_proxyModel->mapToSource(index);
    int row = sourceIndex.row();

    QSqlRecord record = m_model->record(row);

    TaskDialog dialog(this);
    dialog.setTitle(record.value("title").toString());
    dialog.setPriority(record.value("priority").toInt());
    dialog.setDeadline(QDateTime::fromString(record.value("deadline").toString(), "yyyy-MM-dd HH:mm"));
    dialog.setDetail(record.value("detail").toString());

    // Populate categories
    QMap<int, QString> categories;
    QSqlQuery query("SELECT id, name FROM categories", DataManager::instance().getDatabase());
    while (query.next()) {
        categories.insert(query.value(0).toInt(), query.value(1).toString());
    }
    dialog.populateCategories(categories);
    dialog.setCategory(record.value("category_id").toInt());

    if (dialog.exec() == QDialog::Accepted) {
        QSqlQuery query(DataManager::instance().getDatabase());
        query.prepare("UPDATE tasks SET title=?, priority=?, deadline=?, detail=?, category_id=? WHERE id=?");
        query.addBindValue(dialog.getTitle());
        query.addBindValue(dialog.getPriority());
        query.addBindValue(dialog.getDeadline().toString("yyyy-MM-dd HH:mm"));
        query.addBindValue(dialog.getDetail());
        query.addBindValue(dialog.getCategoryId());
        // Get ID from the model record before editing
        query.addBindValue(record.value("id").toInt());

        if (query.exec()) {
            m_model->select(); // Refresh to show joined category name
            if (m_proxyModel) m_proxyModel->invalidate();
        } else {
             QMessageBox::critical(this, tr("错误"), tr("更新失败: ") + query.lastError().text());
        }
    }
}

void MainWindow::deleteTask()
{
    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    if (!index.isValid()) return;

    if (QMessageBox::question(this, tr("确认"), tr("您确定要删除此任务吗？"))
        == QMessageBox::Yes) {
        QModelIndex sourceIndex = m_proxyModel->mapToSource(index);
        m_model->removeRow(sourceIndex.row());
        m_model->submitAll();
    }
}

void MainWindow::checkCloudSync()
{
    m_statusLabel->setText(tr("正在同步..."));
    m_networkManager->checkConnection();
}

void MainWindow::onNetworkStatus(const QString &status)
{
    m_statusLabel->setText(status);
    QMessageBox::information(this, tr("同步状态"), status);
}

void MainWindow::onReminderTriggered(const QString &title, const QString &deadline)
{
    QMessageBox::information(this, tr("提醒"),
                tr("任务 '%1' 已过期！截止时间: %2").arg(title, deadline));
}

void MainWindow::onFilterChanged()
{
    TaskProxyModel *proxy = static_cast<TaskProxyModel*>(m_proxyModel);
    proxy->setPriorityFilter(ui->priorityFilter->currentData().toInt());
    proxy->setStatusFilter(ui->statusFilter->currentData().toInt());

    // For category, if ID is -1, pass empty string
    int catId = ui->categoryFilter->currentData().toInt();
    if (catId == -1) {
        proxy->setCategoryNameFilter("");
    } else {
        proxy->setCategoryNameFilter(ui->categoryFilter->currentText());
    }
}

void MainWindow::exportReport()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出报表"), "",
                                                    tr("PDF 文件 (*.pdf);;Excel 文件 (*.csv)"));
    if (fileName.isEmpty()) return;

    if (fileName.endsWith(".csv")) {
        // Export to CSV
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("错误"), tr("无法保存文件"));
            return;
        }

        QTextStream out(&file);
        // Write BOM for Excel UTF-8 compatibility
        out << "\xEF\xBB\xBF";

        // Statistics
        int total = m_model->rowCount();
        int completed = 0;
        int pending = 0;
        int highPriority = 0;

        for(int i=0; i<total; ++i) {
            int done = m_model->data(m_model->index(i, 3)).toInt();
            int priority = m_model->data(m_model->index(i, 2)).toInt();
            if(done) completed++; else pending++;
            if(priority == 2) highPriority++;
        }

        out << tr("任务统计报表") << "\n";
        out << tr("生成时间: ") << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n\n";
        out << tr("总任务数: ") << total << "\n";
        out << tr("已完成: ") << completed << "\n";
        out << tr("未完成: ") << pending << "\n";
        out << tr("高优先级任务: ") << highPriority << "\n\n";

        // Write headers
        QStringList headers;
        for (int i = 0; i < m_model->columnCount(); ++i) {
            headers << m_model->headerData(i, Qt::Horizontal).toString();
        }
        out << headers.join(",") << "\n";

        // Export all data
        for (int i = 0; i < m_model->rowCount(); ++i) {
            QStringList rowData;
            for (int j = 0; j < m_model->columnCount(); ++j) {
                QString data = m_model->data(m_model->index(i, j)).toString();
                // Escape commas and quotes
                if (data.contains(",") || data.contains("\"")) {
                    data.replace("\"", "\"\"");
                    data = "\"" + data + "\"";
                }
                rowData << data;
            }
            out << rowData.join(",") << "\n";
        }

        file.close();
    } else {
        // Export to PDF
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        QTextDocument doc;
        QString html;
        html += "<h1>" + tr("任务管理统计报表") + "</h1>";
        html += "<p>" + tr("生成时间: ") + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "</p>";


        int total = m_model->rowCount();
        int completed = 0;
        int pending = 0;
        int highPriority = 0;

        for(int i=0; i<total; ++i) {
            int done = m_model->data(m_model->index(i, 3)).toInt();
            int priority = m_model->data(m_model->index(i, 2)).toInt();
            if(done) completed++; else pending++;
            if(priority == 2) highPriority++;
        }

        html += "<h3>" + tr("统计摘要") + "</h3>";
        html += "<ul>";
        html += "<li>" + tr("总任务数: %1").arg(total) + "</li>";
        html += "<li>" + tr("已完成: %1").arg(completed) + "</li>";
        html += "<li>" + tr("未完成: %1").arg(pending) + "</li>";
        html += "<li>" + tr("高优先级任务: %1").arg(highPriority) + "</li>";
        html += "</ul>";

        html += "<h3>" + tr("任务列表") + "</h3>";
        html += "<table border='1' cellspacing='0' cellpadding='4' width='100%'>";


        html += "<thead><tr>";
        for (int i = 0; i < m_model->columnCount(); ++i) {
            // Skip ID column (0)
            if (i == 0) continue;
            html += "<th style='background-color: #f2f2f2;'>" + m_model->headerData(i, Qt::Horizontal).toString() + "</th>";
        }
        html += "</tr></thead>";

        // Data
        html += "<tbody>";
        for (int i = 0; i < m_model->rowCount(); ++i) {
            html += "<tr>";
            for (int j = 0; j < m_model->columnCount(); ++j) {
                if (j == 0) continue;

                QString data = m_model->data(m_model->index(i, j)).toString();
                // Special formatting
                if (j == 2) { // Priority
                    if (data == "0") data = tr("低");
                    else if (data == "1") data = tr("中");
                    else if (data == "2") data = tr("高");
                } else if (j == 3) { // Done
                    data = (data == "1") ? tr("是") : tr("否");
                }

                html += "<td>" + data + "</td>";
            }
            html += "</tr>";
        }
        html += "</tbody></table>";

        doc.setHtml(html);
        doc.print(&printer);
    }

    QMessageBox::information(this, tr("成功"), tr("报表导出成功"));
}





