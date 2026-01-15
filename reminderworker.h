#ifndef REMINDERWORKER_H
#define REMINDERWORKER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>

class ReminderWorker : public QObject
{
    Q_OBJECT
public:
    explicit ReminderWorker(const QString &dbPath, QObject *parent = nullptr);

public slots:
    void startWork();
    void stopWork();
    void checkReminders();

signals:
    void reminderTriggered(const QString &title, const QString &deadline);

private:
    QTimer *m_timer;
    QString m_dbPath;
};

#endif // REMINDERWORKER_H



