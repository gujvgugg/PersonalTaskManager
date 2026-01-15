#include "reminderworker.h"
#include <QDebug>
#include <QSqlError>
#include <QThread>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>

ReminderWorker::ReminderWorker(const QString &dbPath, QObject *parent) 
    : QObject(parent), m_dbPath(dbPath), m_timer(nullptr)
{
}

void ReminderWorker::startWork()
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ReminderWorker::checkReminders);
    m_timer->start(60000);
    

    checkReminders();
}

void ReminderWorker::stopWork()
{
    if (m_timer) {
        m_timer->stop();
    }
}

void ReminderWorker::checkReminders()
{

    QString connectionName = QString("ReminderConnection_%1").arg((quintptr)QThread::currentThreadId());
    {
        QSqlDatabase db;
        if (QSqlDatabase::contains(connectionName)) {
            db = QSqlDatabase::database(connectionName);
        } else {
            db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
            db.setDatabaseName(m_dbPath);
        }

        if (db.open()) {
            QSqlQuery query(db);

            QString nowStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
            
            
            query.prepare("SELECT title, deadline FROM tasks WHERE done = 0 AND deadline <= :now AND deadline > :past");
            query.bindValue(":now", nowStr);
            query.bindValue(":past", QDateTime::currentDateTime().addSecs(-60).toString("yyyy-MM-dd HH:mm"));
            
            if (query.exec()) {
                while (query.next()) {
                    QString title = query.value(0).toString();
                    QString deadline = query.value(1).toString();
                    emit reminderTriggered(title, deadline);
                }
            } else {
                qDebug() << "Reminder query failed:" << query.lastError().text();
            }
            db.close();
        } else {
            qDebug() << "Reminder thread failed to open DB";
        }
    }
    // Remove connection is tricky in threads, usually keep it open or remove carefully. 
    // QSqlDatabase::removeDatabase(connectionName); // Can't remove if open in this scope.
}



