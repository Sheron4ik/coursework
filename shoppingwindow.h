#ifndef SHOPPINGWINDOW_H
#define SHOPPINGWINDOW_H

#include <QWidget>
#include <QMessageBox>

namespace Ui {
class ShoppingWindow;
}

class ShoppingWindow : public QWidget {
    Q_OBJECT

public:
    explicit ShoppingWindow(QWidget *parent = nullptr);
    ~ShoppingWindow();
    bool isFieldsEmpty();

signals:
    void openMainWindow();

private slots:
    void on_back_clicked();
    void on_add_clicked();
    void on_finish_clicked();
    void on_price_valueChanged(double arg1);
    void on_quantity_valueChanged(int arg1);

private:
    Ui::ShoppingWindow *ui;
};

#endif // SHOPPINGWINDOW_H
