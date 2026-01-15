/********************************************************************************
** Form generated from reading UI file 'taskdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TASKDIALOG_H
#define UI_TASKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_TaskDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelTitle;
    QLineEdit *titleEdit;
    QLabel *labelPriority;
    QComboBox *priorityCombo;
    QLabel *labelCategory;
    QComboBox *categoryCombo;
    QLabel *labelDeadline;
    QDateTimeEdit *deadlineEdit;
    QLabel *labelDetail;
    QTextEdit *detailEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TaskDialog)
    {
        if (TaskDialog->objectName().isEmpty())
            TaskDialog->setObjectName("TaskDialog");
        TaskDialog->resize(400, 450);
        verticalLayout = new QVBoxLayout(TaskDialog);
        verticalLayout->setObjectName("verticalLayout");
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        labelTitle = new QLabel(TaskDialog);
        labelTitle->setObjectName("labelTitle");

        formLayout->setWidget(0, QFormLayout::LabelRole, labelTitle);

        titleEdit = new QLineEdit(TaskDialog);
        titleEdit->setObjectName("titleEdit");

        formLayout->setWidget(0, QFormLayout::FieldRole, titleEdit);

        labelPriority = new QLabel(TaskDialog);
        labelPriority->setObjectName("labelPriority");

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPriority);

        priorityCombo = new QComboBox(TaskDialog);
        priorityCombo->addItem(QString());
        priorityCombo->addItem(QString());
        priorityCombo->addItem(QString());
        priorityCombo->setObjectName("priorityCombo");

        formLayout->setWidget(1, QFormLayout::FieldRole, priorityCombo);

        labelCategory = new QLabel(TaskDialog);
        labelCategory->setObjectName("labelCategory");

        formLayout->setWidget(2, QFormLayout::LabelRole, labelCategory);

        categoryCombo = new QComboBox(TaskDialog);
        categoryCombo->setObjectName("categoryCombo");

        formLayout->setWidget(2, QFormLayout::FieldRole, categoryCombo);

        labelDeadline = new QLabel(TaskDialog);
        labelDeadline->setObjectName("labelDeadline");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelDeadline);

        deadlineEdit = new QDateTimeEdit(TaskDialog);
        deadlineEdit->setObjectName("deadlineEdit");
        deadlineEdit->setCalendarPopup(true);

        formLayout->setWidget(3, QFormLayout::FieldRole, deadlineEdit);

        labelDetail = new QLabel(TaskDialog);
        labelDetail->setObjectName("labelDetail");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelDetail);

        detailEdit = new QTextEdit(TaskDialog);
        detailEdit->setObjectName("detailEdit");

        formLayout->setWidget(4, QFormLayout::FieldRole, detailEdit);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(TaskDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(TaskDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, TaskDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, TaskDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(TaskDialog);
    } // setupUi

    void retranslateUi(QDialog *TaskDialog)
    {
        TaskDialog->setWindowTitle(QCoreApplication::translate("TaskDialog", "\344\273\273\345\212\241\350\257\246\346\203\205", nullptr));
        labelTitle->setText(QCoreApplication::translate("TaskDialog", "\346\240\207\351\242\230:", nullptr));
        labelPriority->setText(QCoreApplication::translate("TaskDialog", "\344\274\230\345\205\210\347\272\247:", nullptr));
        priorityCombo->setItemText(0, QCoreApplication::translate("TaskDialog", "\344\275\216", nullptr));
        priorityCombo->setItemText(1, QCoreApplication::translate("TaskDialog", "\344\270\255", nullptr));
        priorityCombo->setItemText(2, QCoreApplication::translate("TaskDialog", "\351\253\230", nullptr));

        labelCategory->setText(QCoreApplication::translate("TaskDialog", "\345\210\206\347\261\273:", nullptr));
        labelDeadline->setText(QCoreApplication::translate("TaskDialog", "\346\210\252\346\255\242\346\227\266\351\227\264:", nullptr));
        deadlineEdit->setDisplayFormat(QCoreApplication::translate("TaskDialog", "yyyy-MM-dd HH:mm", nullptr));
        labelDetail->setText(QCoreApplication::translate("TaskDialog", "\350\257\246\346\203\205:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TaskDialog: public Ui_TaskDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TASKDIALOG_H



