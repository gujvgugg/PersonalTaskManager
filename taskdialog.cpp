#include "taskdialog.h"
#include "ui_taskdialog.h"
#include <QPushButton>

TaskDialog::TaskDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TaskDialog)
{
    ui->setupUi(this);

    // Set UserData for Priority ComboBox items defined in .ui

    ui->priorityCombo->setItemData(0, 0);
    ui->priorityCombo->setItemData(1, 1);
    ui->priorityCombo->setItemData(2, 2);

    ui->deadlineEdit->setDateTime(QDateTime::currentDateTime());

    // Explicitly set button text to Chinese (in case system locale is not Chinese)
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("取消"));
}

TaskDialog::~TaskDialog()
{
    delete ui;
}

void TaskDialog::setTitle(const QString &title)
{
    ui->titleEdit->setText(title);
}

void TaskDialog::setPriority(int priority)
{
    int index = ui->priorityCombo->findData(priority);
    if (index != -1)
        ui->priorityCombo->setCurrentIndex(index);
}

void TaskDialog::setDeadline(const QDateTime &dateTime)
{
    ui->deadlineEdit->setDateTime(dateTime);
}

void TaskDialog::setDetail(const QString &detail)
{
    ui->detailEdit->setText(detail);
}

void TaskDialog::setCategory(int categoryId)
{
    int index = ui->categoryCombo->findData(categoryId);
    if (index != -1)
        ui->categoryCombo->setCurrentIndex(index);
}

void TaskDialog::populateCategories(const QMap<int, QString> &categories)
{
    ui->categoryCombo->clear();
    for (auto it = categories.begin(); it != categories.end(); ++it) {
        ui->categoryCombo->addItem(it.value(), it.key());
    }
}

QString TaskDialog::getTitle() const
{
    return ui->titleEdit->text();
}

int TaskDialog::getPriority() const
{
    return ui->priorityCombo->currentData().toInt();
}

QDateTime TaskDialog::getDeadline() const
{
    return ui->deadlineEdit->dateTime();
}

QString TaskDialog::getDetail() const
{
    return ui->detailEdit->toPlainText();
}

int TaskDialog::getCategoryId() const
{
    return ui->categoryCombo->currentData().toInt();
}



