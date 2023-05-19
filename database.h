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
    void addGoods(const QString& goods, qulonglong idCategory);
    void addPurchase(const QString& goods, const QString& category, const QString& date, const double price);
    void addCategories(const QStringList& categories);
    qulonglong getIDCategory(const QString& category);
    qulonglong getIDGoods(const QString& goods, qulonglong idCategory);
    QStringList* getCategories();
    QStringList* getGoods(const QString& category);
    bool isCategoryEmpty();
    void getAllPurchases();
    QList<QPair<QString, double>> getPurchasesForStatistics(const QString& date);
    QList<QPair<QString, double>> getPurchasesForCategoryStatistic(const QString& category, const QString& date);
};

#endif // DATABASE_H
