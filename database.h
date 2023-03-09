#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>

class Database {
    QSqlDatabase DB;
public:
    Database();
    ~Database();

    void createDB();
    void addCategory(const QString& category);
    void addPurchase(const QString& category, const QString& date, const double price);
    void addCategories(const QStringList& categories);
    qulonglong getIDCategory(const QString& category);
    QStringList* getCategories();
    bool isCategoryEmpty();
    void getPurchases();
};

#endif // DATABASE_H
