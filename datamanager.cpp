#include "datamanager.h"

DataManager::DataManager(QObject *parent) : QObject(parent)
{
    m_dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(m_dbPath);
    if (!dir.exists()) {
        dir.mkpath(m_dbPath);
    }
    m_dbPath += "/tasks_v2.db"; // Use v2 to ensure clean slate
}

DataManager::~DataManager()
{
    closeDatabase();
}

DataManager& DataManager::instance()
{
    static DataManager instance;
    return instance;
}

bool DataManager::openDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        m_db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(m_dbPath);
    }

    if (!m_db.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    } else {
        qDebug() << "Database: connection ok";
        return createTables();
    }
}

void DataManager::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool DataManager::createTables()
{
    QSqlQuery query;

    // Create Categories Table
    bool success = query.exec("CREATE TABLE IF NOT EXISTS categories ("
                              "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "name TEXT UNIQUE NOT NULL)");
    if (!success) {
        qDebug() << "Failed to create categories table:" << query.lastError();
        return false;
    }

    // Insert Default Category if empty
    query.exec("SELECT count(*) FROM categories");
    if (query.next() && query.value(0).toInt() == 0) {
        query.exec("INSERT INTO categories (name) VALUES ('默认')");
        query.exec("INSERT INTO categories (name) VALUES ('工作')");
        query.exec("INSERT INTO categories (name) VALUES ('生活')");
    }





    success = query.exec("CREATE TABLE IF NOT EXISTS tasks ("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "title TEXT NOT NULL, "
                         "priority INTEGER, "
                         "done INTEGER, "
                         "deadline TEXT, "
                         "detail TEXT, "
                         "category_id INTEGER, "
                         "FOREIGN KEY(category_id) REFERENCES categories(id) ON DELETE CASCADE)");

    if (!success) {
        qDebug() << "Failed to create tasks table:" << query.lastError();
        return false;
    }


    query.exec("PRAGMA foreign_keys = ON");

    return true;
}

bool DataManager::addCategory(const QString &name)
{
    QSqlQuery query;
    query.prepare("INSERT INTO categories (name) VALUES (:name)");
    query.bindValue(":name", name);
    return query.exec();
}

bool DataManager::deleteCategory(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM categories WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QStringList DataManager::getAllCategories()
{
    QStringList list;
    QSqlQuery query("SELECT name FROM categories");
    while (query.next()) {
        list << query.value(0).toString();
    }
    return list;
}

QSqlDatabase DataManager::getDatabase() const
{
    return m_db;
}

QString DataManager::getDatabasePath() const
{
    return m_dbPath;
}



