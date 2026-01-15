#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class TaskDialog; }
QT_END_NAMESPACE

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QWidget *parent = nullptr);
    ~TaskDialog();
    
    // Setters to populate data for editing
    void setTitle(const QString &title);
    void setPriority(int priority);
    void setDeadline(const QDateTime &dateTime);
    void setDetail(const QString &detail);
    void setCategory(int categoryId);
    void populateCategories(const QMap<int, QString> &categories); // New

    // Getters to retrieve data
    QString getTitle() const;
    int getPriority() const;
    QDateTime getDeadline() const;
    QString getDetail() const;
    int getCategoryId() const; // New

private:
    Ui::TaskDialog *ui;
};

#endif // TASKDIALOG_H



