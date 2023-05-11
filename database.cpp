#include "database.h"

Database::Database() {
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases");
    if (!DB.open()) {
        qDebug() << "ERROR connection: " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS connection";
    }
    QSqlQuery query(DB);
    if (!query.exec("PRAGMA foreign_keys = ON")) {
        qDebug() << "ERROR pragma: " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS pragma";
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
                    "   id_goods    INTEGER NOT NULL,"
                    "   date        DATE    NOT NULL,"
                    "   price       DECIMAL NOT NULL,"
                    "FOREIGN KEY (id_goods) REFERENCES Goods(id),"
                    "FOREIGN KEY (id_category) REFERENCES Category(id)"
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
    if(!query.exec(
                    "CREATE TABLE IF NOT EXISTS Goods ("
                    "   id          INTEGER PRIMARY KEY,"
                    "   goods       TEXT    UNIQUE,"
                    "   id_category INTEGER NOT NULL,"
                    "FOREIGN KEY (id_category) REFERENCES Category(id)"
                    ")"
                )) {
        qDebug() << "ERROR creation Goods: " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS creation Goods";
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

void Database::addGoods(const QString& goods, qulonglong idCategory) {
    QSqlQuery query(DB);
    query.prepare(
                    "INSERT OR IGNORE INTO Goods (goods, id_category)"
                    "   VALUES (:goods, :idCategory)"
                );
    query.bindValue(":goods", goods);
    query.bindValue(":idCategory", idCategory);
    if (!query.exec()) {
        qDebug() << "ERROR add goods " << goods << idCategory << ": " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS add goods " << goods << idCategory;
    }
}

void Database::addPurchase(const QString& goods, const QString& category, const QString& date, const double price) {
    QSqlQuery query(DB);
    query.prepare(
                    "INSERT INTO Purchases (id_category, id_goods, date, price)"
                    "   VALUES (:id_category, :id_goods, :date, :price)"
                );
    qulonglong idCategory = this->getIDCategory(category);
    if (!idCategory) {
        addCategory(category);
        idCategory = this->getIDCategory(category);
        addGoods(goods, idCategory);
    }
    query.bindValue(":id_category", idCategory);
    query.bindValue(":id_goods", this->getIDGoods(goods, idCategory));
    query.bindValue(":date", date);
    query.bindValue(":price", price);
    if (!query.exec()) {
        qDebug() << "ERROR add purchase " << goods << category << date << price << ": " << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS add purchase " << goods << category << date << price;
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
    qDebug() << "!__ID category__!" << query.value(id).toULongLong();
    return query.value(id).toULongLong();
}

qulonglong Database::getIDGoods(const QString& goods, qulonglong idCategory) {
    QSqlQuery query(DB);
    query.prepare(
                    "SELECT id FROM Goods"
                    "   WHERE (goods = (:goods)) AND"
                    "           (id_category = (:idCategory))"
                );
    query.bindValue(":goods", goods);
    query.bindValue(":idCategory", idCategory);
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

QStringList* Database::getGoods(const QString& category) {
    QSqlQuery query(DB);
    QStringList *goods = new QStringList();
    if (category.isEmpty()) {
        if(!query.exec(
                        "SELECT * FROM Goods"
                    )) {
            qDebug() << "ERROR select goods";
        } else {
            qDebug() << "SUCCESS select goods";
        }
    } else {
        query.prepare(
                        "SELECT * FROM Goods"
                        "   WHERE (id_category = (:idCategory))"
                    );
        query.bindValue(":idCategory", this->getIDCategory(category));
        if(!query.exec()) {
            qDebug() << "ERROR select goods";
        } else {
            qDebug() << "SUCCESS select goods";
        }
    }
    const int id = query.record().indexOf("id");
    const int i_goods = query.record().indexOf("goods");
    const int id_category = query.record().indexOf("id_category");
    while (query.next()) {
        *goods << query.value(i_goods).toString();
        qDebug() << query.value(id).toUInt() << query.value(i_goods).toString() << query.value(id_category).toUInt();
    }
    return goods;
}

bool Database::isCategoryEmpty() {
    QSqlQuery query(DB);
    query.exec(
                    "SELECT * FROM Category"
                );
    return !query.next();
}

void Database::getAllPurchases() {
    QSqlQuery query(DB);
    query.exec(
                    "SELECT * FROM Purchases"
                );
    const int id = query.record().indexOf("id_purchase");
    const int id_category = query.record().indexOf("id_category");
    const int id_goods = query.record().indexOf("id_goods");
    const int date = query.record().indexOf("date");
    const int price = query.record().indexOf("price");
    while (query.next()) {
        qDebug() << query.value(id).toULongLong() << query.value(id_category).toULongLong() << query.value(id_goods).toULongLong() <<
                    query.value(date).toString() << query.value(price).toDouble();
    }
}

QHash<QString, double> Database::getPurchasesForStatistics() {
    QHash<QString, double> ans;
    QSqlQuery query(DB);
    if (!query.exec(
                    "SELECT Category.category, SUM(Purchases.price) FROM Purchases, Category"
                    "   WHERE (Category.id = Purchases.id_category)"
                    "GROUP BY Purchases.id_category"
                )) {
        qDebug() << "ERROR select:" << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS select";
        while (query.next()) {
            qDebug() << query.value(0).toString() << query.value(1).toDouble();
            ans.insert(query.value(0).toString(), query.value(1).toDouble());
        }
    }
    return ans;
}

QHash<QString, double> Database::getPurchasesForCategoryStatistic(const QString& category) {
    QHash<QString, double> ans;
    QSqlQuery query(DB);
    query.prepare(
                    "SELECT Goods.goods, SUM(Purchases.price) FROM Purchases, Goods"
                    "   WHERE (Goods.id = Purchases.id_goods) AND"
                    "           (Purchases.id_category = (:idCategory))"
                    "GROUP BY Purchases.id_goods"
                );
    query.bindValue(":idCategory", this->getIDCategory(category));
    if (!query.exec()) {
        qDebug() << "ERROR select:" << DB.lastError().text();
    } else {
        qDebug() << "SUCCESS select";
        while (query.next()) {
            qDebug() << query.value(0).toString() << query.value(1).toDouble();
            ans.insert(query.value(0).toString(), query.value(1).toDouble());
        }
    }
    return ans;
}

/*
    "CREATE TABLE IF NOT EXISTS Purchases ("
    "   id_purchase INTEGER PRIMARY KEY,"
    "   id_category INTEGER NOT NULL,"
    "   id_goods    INTEGER NOT NULL,"
    "   date        DATE    NOT NULL,"
    "   price       DECIMAL NOT NULL,"

    "CREATE TABLE IF NOT EXISTS Category ("
    "   id          INTEGER PRIMARY KEY,"
    "   category    TEXT    UNIQUE"

    "CREATE TABLE IF NOT EXISTS Goods ("
    "   id          INTEGER PRIMARY KEY,"
    "   goods       TEXT    UNIQUE,"
    "   id_category INTEGER NOT NULL,"


id, name_thing, id_cat
*/
