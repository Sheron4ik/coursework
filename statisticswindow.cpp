#include "statisticswindow.h"
#include "ui_statisticswindow.h"

StatisticsWindow::StatisticsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWindow)
{
    ui->setupUi(this);
    DBstatistic = new Database();

    /*series = new QPieSeries();
    chart = new QChart();
    view = new QChartView(chart);*/
}

StatisticsWindow::~StatisticsWindow() {
    delete ui;
    delete DBstatistic;
    /*delete view;
    delete chart;
    delete series;*/
}

void StatisticsWindow::showStatistic() {
    QPieSeries *series = new QPieSeries();
    QHash<QString, double> info = DBstatistic->getPurchasesForStatistics();
    for (auto [key, value] : info.asKeyValueRange()) {
        series->append(key, value);
        //ui->statistics->setText(ui->statistics->text() + '\n' + key + '\t' + QString::number(value));
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTheme(QChart::ChartThemeBlueIcy);
    QChartView *view = new QChartView(chart);
    view->setParent(ui->graph);

    /*delete view;
    delete chart;
    delete series;*/
    /*
    ui->statistics->setText("Статистика:");
    QHash<QString, double> info = DBstatistic->getPurchasesForStatistics();
    for (auto [key, value] : info.asKeyValueRange()) {
        ui->statistics->setText(ui->statistics->text() + '\n' + key + '\t' + QString::number(value));
    }
    */
}

void StatisticsWindow::on_back_clicked() {
    emit openMainWindow();
}
