#include "shoppingwindow.h"
#include "ui_shoppingwindow.h"

ShoppingWindow::ShoppingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoppingWindow)
{
    ui->setupUi(this);
}

ShoppingWindow::~ShoppingWindow() {
    delete ui;
}

void ShoppingWindow::on_back_clicked() {
    emit openMainWindow();
}
