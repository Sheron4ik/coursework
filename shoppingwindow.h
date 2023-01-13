#ifndef SHOPPINGWINDOW_H
#define SHOPPINGWINDOW_H

#include <QWidget>

namespace Ui {
class ShoppingWindow;
}

class ShoppingWindow : public QWidget {
    Q_OBJECT

public:
    explicit ShoppingWindow(QWidget *parent = nullptr);
    ~ShoppingWindow();

private:
    Ui::ShoppingWindow *ui;
};

#endif // SHOPPINGWINDOW_H
