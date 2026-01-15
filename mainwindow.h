#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QThread>
#include <QLabel>

class ReminderWorker;
class NetworkManager;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTask();
    void editTask();
    void deleteTask();
    void manageCategories(); // New
    void exportReport(); // Renamed
    void onFilterChanged();
    void onReminderTriggered(const QString &title, const QString &deadline); // New
    void onNetworkStatus(const QString &status); // New
    void checkCloudSync(); // New

private:
    void setupModel();
    void setupReminders();
    void setupNetwork();
    void refreshCategories();

    Ui::MainWindow *ui;
    QSqlRelationalTableModel *m_model; // Changed from QSqlTableModel
    QSortFilterProxyModel *m_proxyModel;
    
    // Threading
    QThread *m_workerThread;
    ReminderWorker *m_reminderWorker;

    // Network
    NetworkManager *m_networkManager;
    QLabel *m_statusLabel;
};

#endif // MAINWINDOW_H



