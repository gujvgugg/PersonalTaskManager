#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class CategoryDialog;
}

class CategoryDialog : public QDialog//111
{
    Q_OBJECT

public:
    explicit CategoryDialog(QWidget *parent = nullptr);
    ~CategoryDialog();

private slots:
    void addCategory();
    void deleteCategory();

private:
    Ui::CategoryDialog *ui;
    QSqlTableModel *m_model;
};

#endif // CATEGORYDIALOG_H



