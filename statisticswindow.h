#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QWidget>
#include <database.h>
#include <QtCharts>

namespace Ui {
class StatisticsWindow;
}

class StatisticsWindow : public QWidget {
    Q_OBJECT

public:
    explicit StatisticsWindow(Database *DB, QWidget *parent = nullptr);
    ~StatisticsWindow();
    QPushButton *getShoppingButton();

/*signals:
    void openMainWindow();*/

/*public slots:
    void showStatistic();*/

private slots:
    //void on_back_clicked();
    void showStatistic();
    void showCategoryStatistic(QPieSlice* seriesOfCategory);

private:
    Ui::StatisticsWindow *ui;
    Database *DBstatistic;

    QPieSeries *series;
    QChart *chart;
    QChartView *view;

    Qt::GlobalColor colors[10] = {Qt::red, Qt::gray, Qt::magenta, Qt::darkCyan, Qt::green,
                                Qt::darkMagenta, Qt::yellow, Qt::blue, Qt::cyan, Qt::black};
    int currentColor = 0;
};

#endif // STATISTICSWINDOW_H
