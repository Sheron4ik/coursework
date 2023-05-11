#include "shoppingwindow.h"
#include "ui_shoppingwindow.h"

ShoppingWindow::ShoppingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShoppingWindow)
{
    ui->setupUi(this);

    //ui->category->setEditable(true);
    DBshop = new Database();
    ui->category->addItem("категория продукта...");
    ui->category->addItems(*DBshop->getCategories());
    //delete DBshop;

    productCompleter = new QCompleter(*DBshop->getGoods(""), this);
    productCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->productName->setCompleter(productCompleter);

    //ui->purchase->setReadOnly(true);

    connect(ui->price, SIGNAL(valueChanged(double)), this, SLOT(totalChanged()));
    connect(ui->quantity, SIGNAL(valueChanged(double)), this, SLOT(totalChanged()));
}

ShoppingWindow::~ShoppingWindow() {
    delete ui;
    delete DBshop;
    delete productCompleter;
}

bool ShoppingWindow::isFieldsEmpty() {
    if (ui->productName->displayText() == "название продукта..." ||
        ui->category->currentText() == "категория продукта..." ||
        ui->price->value() == 0.00 ||
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
        if (isFieldsEmpty()) {
            QMessageBox::critical(this, "Ошибка", "Данные некорректны!");
        } else {
            DBshop->addPurchase(ui->productName->text(), ui->category->currentText(),
                                QDate::currentDate().toString("yyyy.MM.dd"), ui->total->text().section(' ', 1, 1).toDouble());
            ui->purchase->setText(ui->productName->displayText() + '\n'
                                  + ui->price->text() + "\tx" + ui->quantity->text() + '\t' + ui->total->text()
                                  + "\n\n" + ui->purchase->toPlainText());
            ui->category->clear();
            ui->category->addItem("категория продукта...");
            ui->category->addItems(*DBshop->getCategories());
            delete productCompleter;
            productCompleter = new QCompleter(*DBshop->getGoods(""), ui->productName);
            productCompleter->setCaseSensitivity(Qt::CaseInsensitive);
            //ui->productName->setCompleter(productCompleter);
        }
    }
}

void ShoppingWindow::on_finish_clicked() {
    QMessageBox::StandardButton answer = QMessageBox::question(this, "Завершить", "Вы уверены, что всё добавили?");
    if (answer == QMessageBox::Yes) {
        if (ui->purchase->toPlainText().isEmpty()) {
            QMessageBox::critical(this, "Ошибка", "Ничего не добавлено!");
        } else {
            ui->purchase->clear();
            emit openMainWindow();
        }
    }
}

void ShoppingWindow::totalChanged() {
    double ans = ui->price->value() * ui->quantity->value();
    ui->total->setText("ИТОГО: " + QString::number(ans, 'g', 8));
}

void ShoppingWindow::on_category_currentTextChanged(const QString &category) {
    delete productCompleter;
    productCompleter = new QCompleter(*DBshop->getGoods(category), this);
    productCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    ui->productName->setCompleter(productCompleter);
}
