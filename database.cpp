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

void Database::addPurchase(const QString& category, const QString& date, const double price) {
    QSqlQuery query(DB);
    query.prepare(
                    "INSERT INTO Purchases (id_category, date, price)"
                    "   VALUES (:id_category, :date, :price)"
                );
    query.bindValue(":id_category", this->getIDCategory(category));
    query.bindValue(":date", date);
    query.bindValue(":price", price);
    if (!query.exec()) {
        qDebug() << "ERROR add purchase " << category << date << price << ": " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS add purchase " << category << date << price;
    }
}

void Database::addCategories(const QStringList& categories) {
    foreach (const QString category, categories) {
        addCategory(category);
    }
}

qulonglong Database::getIDCategory(const QString& category) {
    QSqlQuery query(DB);
    query.prepare(
                    "SELECT id FROM Category"
                    "   WHERE category = (:category)"
                );
    query.bindValue(":category", category);
    query.exec();
    const int id = query.record().indexOf("id");
    query.next();
    return query.value(id).toULongLong();
}

QStringList* Database::getCategories() {
    QSqlQuery query(DB);
    query.exec(
                    "SELECT * FROM Category"
                );
    QStringList *categories = new QStringList();
    const int id = query.record().indexOf("id");
    const int category = query.record().indexOf("category");
    while (query.next()) {
        *categories << query.value(category).toString();
        qDebug() << query.value(id).toUInt() << query.value(category).toString();
    }
    return categories;
}

bool Database::isCategoryEmpty() {
    QSqlQuery query(DB);
    query.exec(
                    "SELECT * FROM Category"
                );
    return !query.next();
}

void Database::getPurchases() {
    QSqlQuery query(DB);
    query.exec(
                    "SELECT * FROM Purchases"
                );
    const int id = query.record().indexOf("id_purchase");
    const int id_category = query.record().indexOf("id_category");
    const int date = query.record().indexOf("date");
    const int price = query.record().indexOf("price");
    while (query.next()) {
        qDebug() << query.value(id).toULongLong() << query.value(id_category).toULongLong() <<
                    query.value(date).toString() << query.value(price).toDouble();
    }
}
