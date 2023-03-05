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
    void addPurchase(const QString& category, const QString& date, const QString& price);
    void addCategories(const QStringList& categories);
    QStringList* getCategories();
    bool isCategoryEmpty();
};

#endif // DATABASE_H
