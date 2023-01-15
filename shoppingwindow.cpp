#include "shoppingwindow.h"
#include "ui_shoppingwindow.h"

ShoppingWindow::ShoppingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoppingWindow)
{
    ui->setupUi(this);

    ui->category->setEditable(true);

    ui->purchase->setReadOnly(true);
}

ShoppingWindow::~ShoppingWindow() {
    delete ui;
}

bool ShoppingWindow::isFieldsEmpty() {
    if (ui->productName->displayText().isEmpty() ||
        ui->category->currentText().isEmpty() ||
        ui->price->value() == 0.0 ||
        ui->quantity->value() == 0)
        return true;
    return false;
}

void ShoppingWindow::on_back_clicked() {
    emit openMainWindow();
}

void ShoppingWindow::on_add_clicked() {
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Добавить", "Вы уверены, что данные корректны?");
    if (answer == QMessageBox::Yes) {
        if (isFieldsEmpty())
            QMessageBox::critical(this, "Ошибка", "Данные некорректны!");
        else
            QMessageBox::information(this, "Да", "Да");
    }
}

void ShoppingWindow::on_finish_clicked() {
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Завершить", "Вы уверены, что всё добавили?");
    if (answer == QMessageBox::Yes) {
        QMessageBox::information(this, "Да", "Да");
    } else {
        QMessageBox::information(this, "Нет", "Нет");
    }
}

void ShoppingWindow::on_price_valueChanged(double arg1) {
    double ans = arg1 * double(ui->quantity->value());
    ui->total->setText(QString::number(ans, 'g', 8));
}

void ShoppingWindow::on_quantity_valueChanged(int arg1) {
    double ans = double(arg1) * ui->price->value();
    ui->total->setText(QString::number(ans, 'g', 8));
}
