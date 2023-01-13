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

}

MainWindow::~MainWindow() {
    delete ui;
    delete shoppingWindow;
    delete statisticsWindow;
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
