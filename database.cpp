#include "database.h"

Database::Database() {
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases");
    if (!DB.open()) {
        qDebug() << "ERROR connection: " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS connection";
    }
}

Database::~Database() {
    DB.close();
    QFile::remove(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases");
    qDebug() << "SUCCESS closing";
}

void Database::createDB() {
    QSqlQuery query(DB);
    if(!query.exec(
                    "CREATE TABLE IF NOT EXISTS Purchases ("
                    "   id_purchase INTEGER PRIMARY KEY,"
                    "   id_category INTEGER NOT NULL,"
                    "   date        DATE    NOT NULL,"
                    "   price       DECIMAL NOT NULL"
                    ")"
                )) {
        qDebug() << "ERROR creation Purchases: " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS creation Purchases";
    }
    if(!query.exec(
                    "CREATE TABLE IF NOT EXISTS Category ("
                    "   id          INTEGER PRIMARY KEY,"
                    "   category    TEXT    UNIQUE"
                    ")"
                )) {
        qDebug() << "ERROR creation Category: " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS creation Category";
    }
}

void Database::addCategory(const QString& category) {
    QSqlQuery query(DB);
    query.prepare(
                    "INSERT OR IGNORE INTO Category (category)"
                    "   VALUES (:category)"
                );
    query.bindValue(":category", category);
    if (!query.exec()) {
        qDebug() << "ERROR add category " << category << ": " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS add category " << category;
    }
}

void Database::addCategories(const QStringList& categories) {
    foreach (const QString category, categories) {
        addCategory(category);
    }
}

void Database::getCategories() {
    QSqlQuery query(DB);
    if (!query.exec(
                    "SELECT * FROM Category"
                )) {
        qDebug() << "ERROR selection Category: " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS selection Category";
        const int id = query.record().indexOf("id");
        const int category = query.record().indexOf("category");
        while (query.next()) {
            qDebug() << query.value(id).toUInt() << query.value(category).toString();
        }
    }
}
