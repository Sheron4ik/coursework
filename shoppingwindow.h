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
    void totalChanged();

private:
    Ui::ShoppingWindow *ui;
    quint8 numberOfPurchases;
};

#endif // SHOPPINGWINDOW_H
