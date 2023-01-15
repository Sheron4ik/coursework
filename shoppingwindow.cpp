#include "shoppingwindow.h"
#include "ui_shoppingwindow.h"

ShoppingWindow::ShoppingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoppingWindow)
{
    ui->setupUi(this);

    ui->category->setEditable(true);
}

ShoppingWindow::~ShoppingWindow() {
    delete ui;
}

void ShoppingWindow::on_back_clicked() {
    emit openMainWindow();
}

void ShoppingWindow::on_add_clicked() {
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Подтверждение", "Вы уверены, что данные корректны?");
    if (answer == QMessageBox::Yes) {
        QMessageBox::information(this, "Да", "Да");
    } else {
        QMessageBox::information(this, "Нет", "Нет");
    }
}
