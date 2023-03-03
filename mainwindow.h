#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <database.h>
//#include <QtSql>
//#include <QFileInfo>
//#include <QMessageBox>

#include "shoppingwindow.h"
#include "statisticswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_help_clicked();
    void on_buy_clicked();
    void on_statistics_clicked();
    void backMainWindow();

private:
    Ui::MainWindow *ui;
    ShoppingWindow *shoppingWindow;
    StatisticsWindow *statisticsWindow;
    Database *DBpurchases;
    /*QSqlDatabase DBpurchases;

    bool createDB();
    bool addCategoryIntoDB(const QString& category);
    void selectCategories();*/
};
#endif // MAINWINDOW_H
