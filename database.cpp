#include "database.h"

Database::Database() {
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases.db");
    DB.open();
    QSqlQuery query(DB);
    query.exec("PRAGMA foreign_keys = ON");
}

Database::~Database() {
    DB.close();
}

void Database::createDB() {
    QSqlQuery query(DB);
    query.exec(
                    "CREATE TABLE IF NOT EXISTS Purchases ("
                    "   id_purchase INTEGER PRIMARY KEY,"
                    "   id_category INTEGER NOT NULL,"
                    "   id_goods    INTEGER NOT NULL,"
                    "   date        DATE    NOT NULL,"
                    "   price       DECIMAL NOT NULL,"
                    "FOREIGN KEY (id_goods) REFERENCES Goods(id),"
                    "FOREIGN KEY (id_category) REFERENCES Category(id)"
                    ")"
                );

    query.exec(
                    "CREATE TABLE IF NOT EXISTS Category ("
                    "   id          INTEGER PRIMARY KEY,"
                    "   category    TEXT    UNIQUE"
                    ")"
                );

    query.exec(
                    "CREATE TABLE IF NOT EXISTS Goods ("
                    "   id          INTEGER PRIMARY KEY,"
                    "   goods       TEXT    UNIQUE,"
                    "   id_category INTEGER NOT NULL,"
                    "FOREIGN KEY (id_category) REFERENCES Category(id)"
                    ")"
                );

    if (isCategoryEmpty()) {
        addCategories({"Продукты", "Книги", "Аптеки", "Электроника", "Украшения", "Канцелярия", "Алкоголь"});
        addGoods("Хлеб", 1);
        addGoods("Война и Мир", 2);
        addGoods("Аскорбинка", 3);
        addGoods("Арпефлю", 3);
        addGoods("Серьги", 5);
        addGoods("Ручка", 6);
        addGoods("Карандаш", 6);
        addGoods("Ластик", 6);
        addGoods("Жигулевское", 7);
    }
}

void Database::addCategory(const QString& category) {
    QSqlQuery query(DB);
    query.prepare(
                    "INSERT OR IGNORE INTO Category (category)"
                    "   VALUES (:category)"
                );
    query.bindValue(":category", category);
    query.exec();
}

void Database::addGoods(const QString& goods, qulonglong idCategory) {
    QSqlQuery query(DB);
    query.prepare(
                    "INSERT OR IGNORE INTO Goods (goods, id_category)"
                    "   VALUES (:goods, :idCategory)"
                );
    query.bindValue(":goods", goods);
    query.bindValue(":idCategory", idCategory);
    query.exec();
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
    }
    if (!this->getIDGoods(goods, idCategory)) {
        addGoods(goods, idCategory);
    }
    query.bindValue(":id_category", idCategory);
    query.bindValue(":id_goods", this->getIDGoods(goods, idCategory));
    query.bindValue(":date", date);
    query.bindValue(":price", price);
    query.exec();
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
                    "SELECT category FROM Category"
                );
    QStringList *categories = new QStringList();
    const int category = query.record().indexOf("category");
    while (query.next()) {
        *categories << query.value(category).toString();
    }
    return categories;
}

QStringList* Database::getGoods(const QString& category) {
    QSqlQuery query(DB);
    QStringList *goods = new QStringList();
    if (category.isEmpty()) {
        query.exec(
                        "SELECT goods FROM Goods"
                    );
    } else {
        query.prepare(
                        "SELECT goods FROM Goods"
                        "   WHERE (id_category = (:idCategory))"
                    );
        query.bindValue(":idCategory", this->getIDCategory(category));
        query.exec();
    }
    const int i_goods = query.record().indexOf("goods");
    while (query.next()) {
        *goods << query.value(i_goods).toString();
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

QList<QPair<QString, double>> Database::getPurchasesForStatistics(const QString& date) {
    QList<QPair<QString, double>> ans;
    QSqlQuery query(DB);
    query.prepare(
                    "SELECT Category.category, SUM(Purchases.price) FROM Purchases, Category"
                    "   WHERE (Category.id = Purchases.id_category) AND"
                    "           ((:date) <= Purchases.date)"
                    " GROUP BY Purchases.id_category"
                    " ORDER BY SUM(Purchases.price) DESC"
                );
    query.bindValue(":date", date);
    query.exec();
    while (query.next()) {
        ans.append(qMakePair(query.value(0).toString(), query.value(1).toDouble()));
    }
    return ans;
}

QList<QPair<QString, double>> Database::getPurchasesForCategoryStatistic(const QString& category, const QString& date) {
    QList<QPair<QString, double>> ans;
    QSqlQuery query(DB);
    query.prepare(
                    "SELECT Goods.goods, SUM(Purchases.price) FROM Purchases, Goods"
                    "   WHERE (Goods.id = Purchases.id_goods) AND"
                    "           (Purchases.id_category = (:idCategory)) AND"
                    "           ((:date) <= Purchases.date)"
                    " GROUP BY Purchases.id_goods"
                    " ORDER BY SUM(Purchases.price) DESC"
                );
    query.bindValue(":idCategory", this->getIDCategory(category));
    query.bindValue(":date", date);
    query.exec();
    while (query.next()) {
        ans.append(qMakePair(query.value(0).toString(), query.value(1).toDouble()));
    }
    return ans;
}
