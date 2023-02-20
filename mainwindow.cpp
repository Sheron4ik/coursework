#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    shoppingWindow = new ShoppingWindow();
    connect(shoppingWindow, SIGNAL(openMainWindow()), this, SLOT(backMainWindow()));

    statisticsWindow = new StatisticsWindow();
    connect(statisticsWindow, SIGNAL(openMainWindow()), this, SLOT(backMainWindow()));

    DBpurchases = QSqlDatabase::addDatabase("QSQLITE");
    if (!QFile::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases")) {
        DBpurchases.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases");
        QSqlQuery query(DBpurchases);
        if( !query.exec(
                        "CREATE TABLE Purchases("
                        "   category TEXT PRIMARY KEY NOT NULL,"
                        "   date DATE NOT NULL"
                        "   price DECIMAL"
                        ")"
                        )
            ) {
                QMessageBox::critical(this, "Ошибка", "что-то не получилось!");
            }
    } else {
        DBpurchases.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases");
        QMessageBox::critical(this, "Ошибка", "БД уже существует!");
        //QFile::remove(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases");
    }
    if (!DBpurchases.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к БД!");
    } else {
        QMessageBox::information(this, "Успешно", "Подключение к БД настроено)");
    }
}

MainWindow::~MainWindow() {
    delete ui;
    delete shoppingWindow;
    delete statisticsWindow;
    DBpurchases.close();
    //QFile::remove(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases");
}

void MainWindow::on_help_clicked() {
    QMessageBox::information(this, "Помощь",
                                 "Привет!\n"
                                 "В этом приложении ты можешь контролировать свои расходы, просто добавляй свои покупки =)\n"
                                 "В нижней части экрана есть две кнопки. По левой кнопке ты смотришь свою статистику, по правой - начинаешь добавлять свою покупку.\n"
                                 "Всё просто! Успехов :D");
}

void MainWindow::on_buy_clicked() {
    shoppingWindow->show();
    this->setVisible(false);
}

void MainWindow::on_statistics_clicked() {
    statisticsWindow->show();
    this->setVisible(false);
}

void MainWindow::backMainWindow() {
    this->setVisible(true);
    statisticsWindow->setVisible(false);
    shoppingWindow->setVisible(false);
}
