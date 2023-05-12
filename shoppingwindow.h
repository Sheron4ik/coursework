#ifndef SHOPPINGWINDOW_H
#define SHOPPINGWINDOW_H

#include <QWidget>
#include <database.h>
#include <QMessageBox>
#include <QCompleter>

namespace Ui {
class ShoppingWindow;
}

class ShoppingWindow : public QWidget {
    Q_OBJECT

public:
    explicit ShoppingWindow(Database *DB, QWidget *parent = nullptr);
    ~ShoppingWindow();
    bool isFieldsEmpty();
    //QPushButton *getStatisticsButton();

signals:
    void openStatisticsWindow();

private slots:
    void on_back_clicked();
    void on_add_clicked();
    void on_finish_clicked();
    void totalChanged();
    void on_category_currentTextChanged(const QString &category);

private:
    Ui::ShoppingWindow *ui;
//    quint8 numberOfPurchases;
    Database *DBshop;
    QCompleter *productCompleter;
};

#endif // SHOPPINGWINDOW_H
