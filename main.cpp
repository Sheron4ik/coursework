//#include "mainwindow.h"
#include "shoppingwindow.h"
#include "statisticswindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    /*MainWindow window;
    window.show();*/
    Database *DB = new Database();
    DB->createDB();
    if (DB->isCategoryEmpty()) {
        DB->addCategories({"Продукты", "Книги", "Аптеки", "Электроника", "Украшения", "Канцелярия", "Алкоголь"});
        DB->addGoods("Хлеб", 1);
        DB->addGoods("Война и Мир", 2);
        DB->addGoods("Аскорбинка", 3);
        DB->addGoods("Арпефлю", 3);
        DB->addGoods("Серьги", 5);
        DB->addGoods("Ручка", 6);
        DB->addGoods("Карандаш", 6);
        DB->addGoods("Ластик", 6);
        DB->addGoods("Жигулевское", 7);
    }
    ShoppingWindow shopWindow(DB);
    StatisticsWindow statWindow(DB);

    QObject::connect(&shopWindow, SIGNAL(openStatisticsWindow()), &statWindow, SLOT(showStatistic()));
    QObject::connect(statWindow.getShoppingButton(), &QPushButton::clicked,
                     [&statWindow]() { statWindow.hide(); });

    shopWindow.show();
    return a.exec();
}
