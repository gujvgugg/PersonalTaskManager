/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *filterLayout;
    QComboBox *priorityFilter;
    QComboBox *statusFilter;
    QComboBox *categoryFilter;
    QSpacerItem *horizontalSpacer;
    QPushButton *syncButton;
    QHBoxLayout *actionLayout;
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *categoryButton;
    QPushButton *exportButton;
    QTableView *tableView;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(809, 499);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        filterLayout = new QHBoxLayout();
        filterLayout->setObjectName("filterLayout");
        priorityFilter = new QComboBox(centralwidget);
        priorityFilter->setObjectName("priorityFilter");

        filterLayout->addWidget(priorityFilter);

        statusFilter = new QComboBox(centralwidget);
        statusFilter->setObjectName("statusFilter");

        filterLayout->addWidget(statusFilter);

        categoryFilter = new QComboBox(centralwidget);
        categoryFilter->setObjectName("categoryFilter");

        filterLayout->addWidget(categoryFilter);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        filterLayout->addItem(horizontalSpacer);

        syncButton = new QPushButton(centralwidget);
        syncButton->setObjectName("syncButton");

        filterLayout->addWidget(syncButton);


        verticalLayout->addLayout(filterLayout);

        actionLayout = new QHBoxLayout();
        actionLayout->setObjectName("actionLayout");
        addButton = new QPushButton(centralwidget);
        addButton->setObjectName("addButton");

        actionLayout->addWidget(addButton);

        editButton = new QPushButton(centralwidget);
        editButton->setObjectName("editButton");

        actionLayout->addWidget(editButton);

        deleteButton = new QPushButton(centralwidget);
        deleteButton->setObjectName("deleteButton");

        actionLayout->addWidget(deleteButton);

        categoryButton = new QPushButton(centralwidget);
        categoryButton->setObjectName("categoryButton");

        actionLayout->addWidget(categoryButton);

        exportButton = new QPushButton(centralwidget);
        exportButton->setObjectName("exportButton");

        actionLayout->addWidget(exportButton);


        verticalLayout->addLayout(actionLayout);

        tableView = new QTableView(centralwidget);
        tableView->setObjectName("tableView");
        tableView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
        tableView->setSortingEnabled(true);
        tableView->horizontalHeader()->setStretchLastSection(true);

        verticalLayout->addWidget(tableView);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\344\270\252\344\272\272\344\273\273\345\212\241\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        syncButton->setText(QCoreApplication::translate("MainWindow", "\344\272\221\345\220\214\346\255\245\346\243\200\346\237\245", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\344\273\273\345\212\241", nullptr));
        editButton->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221\344\273\273\345\212\241", nullptr));
        deleteButton->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\344\273\273\345\212\241", nullptr));
        categoryButton->setText(QCoreApplication::translate("MainWindow", "\345\210\206\347\261\273\347\256\241\347\220\206", nullptr));
        exportButton->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\346\212\245\350\241\250", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H



