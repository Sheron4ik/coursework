#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DBmain = new Database();
    DBmain->createDB();
    if (DBmain->isCategoryEmpty()) {
        DBmain->addCategories({"Продукты", "Книги", "Аптеки", "Электроника", "Украшения", "Канцелярия", "Алкоголь"});
        DBmain->addGoods("Хлеб", 1);
        DBmain->addGoods("Война и Мир", 2);
        DBmain->addGoods("Аскорбинка", 3);
        DBmain->addGoods("Арпефлю", 3);
        DBmain->addGoods("Серьги", 5);
        DBmain->addGoods("Ручка", 6);
        DBmain->addGoods("Карандаш", 6);
        DBmain->addGoods("Ластик", 6);
        DBmain->addGoods("Жигулевское", 7);
    }

    shoppingWindow = new ShoppingWindow();
    connect(shoppingWindow, SIGNAL(openMainWindow()), this, SLOT(backMainWindow()));

    statisticsWindow = new StatisticsWindow();
    connect(statisticsWindow, SIGNAL(openMainWindow()), this, SLOT(backMainWindow()));

    /*if (!QFile::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases")) {
        QMessageBox::information(this, "exists false", QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases");
    } else {
        QMessageBox::information(this, "", "exists true");
    }

    if (!QFileInfo::exists(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/dbPurchases")) {
        QMessageBox::information(this, "", "БД не существует!)");
    } else {
        QMessageBox::critical(this, "", "БД уже существует!");
    }*/

    qDebug() << QDate::currentDate().toString("yyyy.MM.dd");
    qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

MainWindow::~MainWindow() {
    delete statisticsWindow;
    delete shoppingWindow;
    delete DBmain;
    delete ui;
    qDebug() << QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
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
    //statisticsWindow->showStatistic();
    statisticsWindow->show();
    this->setVisible(false);
}

void MainWindow::backMainWindow() {
    this->setVisible(true);
    statisticsWindow->setVisible(false);
    shoppingWindow->setVisible(false);
    DBmain->getAllPurchases();
}
