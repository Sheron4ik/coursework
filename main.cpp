#include "shoppingwindow.h"
#include "statisticswindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Database *DB = new Database();
    DB->createDB();

    ShoppingWindow shopWindow(DB);
    StatisticsWindow statWindow(DB);

    QObject::connect(&shopWindow, SIGNAL(openStatisticsWindow()), &statWindow, SLOT(showStatistic()));
    QObject::connect(statWindow.getShoppingButton(), &QPushButton::clicked,
                     [&statWindow]() { statWindow.hide(); });

    shopWindow.show();
    return a.exec();
}
