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

private slots:
    void showStatistic();
    void showCategoryStatistic(QPieSlice* seriesOfCategory);
    void on_settings_currentTextChanged(const QString& setting);

private:
    Ui::StatisticsWindow *ui;
    Database *DBstatistic;

    QPieSeries *series;
    QChart *chart;
    QChartView *view;

    QDate date;

    Qt::GlobalColor colors[10] = {Qt::red, Qt::gray, Qt::magenta, Qt::darkCyan, Qt::green,
                                Qt::darkMagenta, Qt::yellow, Qt::blue, Qt::cyan, Qt::black};
    int currentColor = 0;
};

#endif // STATISTICSWINDOW_H
