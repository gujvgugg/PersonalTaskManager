/********************************************************************************
** Form generated from reading UI file 'categorydialog.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CATEGORYDIALOG_H
#define UI_CATEGORYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CategoryDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *closeButton;

    void setupUi(QDialog *CategoryDialog)
    {
        if (CategoryDialog->objectName().isEmpty())
            CategoryDialog->setObjectName("CategoryDialog");
        CategoryDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(CategoryDialog);
        verticalLayout->setObjectName("verticalLayout");
        tableView = new QTableView(CategoryDialog);
        tableView->setObjectName("tableView");
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        addButton = new QPushButton(CategoryDialog);
        addButton->setObjectName("addButton");

        horizontalLayout->addWidget(addButton);

        deleteButton = new QPushButton(CategoryDialog);
        deleteButton->setObjectName("deleteButton");

        horizontalLayout->addWidget(deleteButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        closeButton = new QPushButton(CategoryDialog);
        closeButton->setObjectName("closeButton");

        horizontalLayout->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(CategoryDialog);

        QMetaObject::connectSlotsByName(CategoryDialog);
    } // setupUi

    void retranslateUi(QDialog *CategoryDialog)
    {
        CategoryDialog->setWindowTitle(QCoreApplication::translate("CategoryDialog", "\345\210\206\347\261\273\347\256\241\347\220\206", nullptr));
        addButton->setText(QCoreApplication::translate("CategoryDialog", "\346\267\273\345\212\240\345\210\206\347\261\273", nullptr));
        deleteButton->setText(QCoreApplication::translate("CategoryDialog", "\345\210\240\351\231\244\345\210\206\347\261\273", nullptr));
        closeButton->setText(QCoreApplication::translate("CategoryDialog", "\345\205\263\351\227\255", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CategoryDialog: public Ui_CategoryDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CATEGORYDIALOG_H



