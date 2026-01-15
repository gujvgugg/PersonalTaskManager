#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

class DataManager : public QObject
{
    Q_OBJECT
public:
    static DataManager& instance();
    bool openDatabase();
    void closeDatabase();
    bool createTables();
    

    bool addCategory(const QString &name);
    bool deleteCategory(int id);
    QStringList getAllCategories();
    
    QSqlDatabase getDatabase() const;
    QString getDatabasePath() const;

private:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();
    QSqlDatabase m_db;
    QString m_dbPath;
};

#endif // DATAMANAGER_H



