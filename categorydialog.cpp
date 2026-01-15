#include "categorydialog.h"
#include "ui_categorydialog.h"
#include "datamanager.h"
#include <QMessageBox>
#include <QInputDialog>

CategoryDialog::CategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategoryDialog)
{
    ui->setupUi(this);

    m_model = new QSqlTableModel(this, DataManager::instance().getDatabase());
    m_model->setTable("categories");
    m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("分类名称"));
    m_model->select();

    ui->tableView->setModel(m_model);
    ui->tableView->hideColumn(0); // Hide ID
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    connect(ui->addButton, &QPushButton::clicked, this, &CategoryDialog::addCategory);
    connect(ui->deleteButton, &QPushButton::clicked, this, &CategoryDialog::deleteCategory);
    connect(ui->closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
}

void CategoryDialog::addCategory()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("添加分类"),
                                         tr("分类名称:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        if (!DataManager::instance().addCategory(text)) {
            QMessageBox::warning(this, tr("错误"), tr("添加分类失败，可能是名称重复"));
        } else {
            m_model->select();
        }
    }
}

void CategoryDialog::deleteCategory()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, tr("提示"), tr("请选择要删除的分类"));
        return;
    }

    int row = index.row();
    int id = m_model->data(m_model->index(row, 0)).toInt();
    QString name = m_model->data(m_model->index(row, 1)).toString();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("确认删除"),
                                  tr("确定要删除分类 '%1' 吗？\n这将同时删除该分类下的所有任务！").arg(name),
                                  QMessageBox::Yes|QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // DataManager handles the logic if we want manual control, 
        // but here we can just use the model or DataManager.
        // Since we enabled ON DELETE CASCADE in DB schema, deleting category should delete tasks.
        
        if (!DataManager::instance().deleteCategory(id)) {
             QMessageBox::warning(this, tr("错误"), tr("删除分类失败"));
        } else {
            m_model->select();
        }
    }
}



