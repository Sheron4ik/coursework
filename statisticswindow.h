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
    explicit StatisticsWindow(QWidget *parent = nullptr);
    ~StatisticsWindow();

signals:
    void openMainWindow();

public slots:
    void showStatistic();

private slots:
    void on_back_clicked();
    //void showStatistic();
    void showCategoryStatistic(QPieSlice* seriesOfCategory);

private:
    Ui::StatisticsWindow *ui;
    Database *DBstatistic;

    QPieSeries *series;
    QChart *chart;
    QChartView *view;
};

#endif // STATISTICSWINDOW_H
