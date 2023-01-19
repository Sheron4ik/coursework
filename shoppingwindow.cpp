#include "shoppingwindow.h"
#include "ui_shoppingwindow.h"

ShoppingWindow::ShoppingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoppingWindow)
{
    ui->setupUi(this);

    //ui->category->setEditable(true);

    //ui->purchase->setReadOnly(true);

    connect(ui->price, SIGNAL(valueChanged(double)), this, SLOT(totalChanged()));
    connect(ui->quantity, SIGNAL(valueChanged(double)), this, SLOT(totalChanged()));
}

ShoppingWindow::~ShoppingWindow() {
    delete ui;
}

bool ShoppingWindow::isFieldsEmpty() {
    if (ui->productName->displayText() == "название продукта..." ||
        ui->category->currentText() == "категория продукта..." ||
        ui->price->value() == 0.0 ||
        ui->quantity->value() == 0)
        return true;
    return false;
}

void ShoppingWindow::on_back_clicked() {
    if (ui->purchase->toPlainText().isEmpty())
        emit openMainWindow();
    else
        QMessageBox::critical(this, "Ошибка", "Завершите покупку!");
}

void ShoppingWindow::on_add_clicked() {
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Добавить", "Вы уверены, что данные корректны?");
    if (answer == QMessageBox::Yes) {
        if (isFieldsEmpty())
            QMessageBox::critical(this, "Ошибка", "Данные некорректны!");
        else
            ui->purchase->setText(ui->purchase->toPlainText() + ui->productName->displayText() + '\n'
                                  + ui->price->text() + "\tx" + ui->quantity->text() + '\t'
                                  + ui->total->text() + "\n\n");
    }
}

void ShoppingWindow::on_finish_clicked() {
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Завершить", "Вы уверены, что всё добавили?");
    if (answer == QMessageBox::Yes) {
        if (ui->purchase->toPlainText().isEmpty())
            QMessageBox::critical(this, "Ошибка", "Ничего не добавлено!");
        else
            emit openMainWindow();
    }
}

void ShoppingWindow::totalChanged() {
    double ans = ui->price->value() * ui->quantity->value();
    ui->total->setText("ИТОГО: " + QString::number(ans, 'g', 8));
}
