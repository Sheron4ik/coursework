#include "statisticswindow.h"
#include "ui_statisticswindow.h"

StatisticsWindow::StatisticsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWindow)
{
    ui->setupUi(this);
    DBstatistic = new Database();

    series = new QPieSeries();
//TODO
    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignLeft);
    chart->legend()->detachFromChart();
    chart->legend()->setInteractive(true);
    chart->legend()->setBackgroundVisible(false || true);
    chart->legend()->setMaximumSize(350, 435);
    chart->legend()->setGeometry(7, 250, 350, 435);
    chart->legend()->update();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setBackgroundVisible(false || true);

    view = new QChartView(chart);
    ui->graph->layout()->addWidget(view);
}

StatisticsWindow::~StatisticsWindow() {
    delete ui;
    delete DBstatistic;
    ui->graph->layout()->removeWidget(view);
    delete view;
}

void StatisticsWindow::showStatistic() {
    chart->removeSeries(series);
    delete series;
    series = new QPieSeries();
    QHash<QString, double> info = DBstatistic->getPurchasesForStatistics();
    qDebug() << "info map:";
    for (auto [key, value] : info.asKeyValueRange()) {
        series->append(key, value);
        series->setName(key);
        qDebug() << key << value << series->name();
    }
    series->setVerticalPosition(0.17);
    for (auto slice : series->slices()) {
        slice->setLabel(slice->label() + ' ' + QString::number(slice->value()) + " руб.");
    }
    connect(series, SIGNAL(clicked(QPieSlice*)), this, SLOT(showCategoryStatistic(QPieSlice*)));
    chart->addSeries(series);
    //chart->setTheme(QChart::ChartThemeBlueIcy);

    /*//view->setRenderHint(QPainter::Antialiasing);
    //view->setAlignment(Qt::AlignBottom);
    //view->setSizePolicy(ui->graph->sizePolicy());
    //view->fitInView(ui->graph->frameRect());
    //view->setParent(ui->graph);*/

    qDebug() << "chart " << chart->x() << chart->y() << chart->size() << chart->geometry();
    qDebug() << "chart legend " << chart->legend()->x() << chart->legend()->y() << chart->legend()->size() << chart->legend()->geometry();
    qDebug() << "graph " << ui->graph->x() << ui->graph->y() << ui->graph->size() << ui->graph->geometry();
    qDebug() << "pieSize" << series->pieSize();
}

void StatisticsWindow::showCategoryStatistic(QPieSlice* seriesOfCategory) {
    chart->removeSeries(series);
    delete series;
    series = new QPieSeries();
    QHash<QString, double> info = DBstatistic->getPurchasesForCategoryStatistic(seriesOfCategory->label().section(' ', 0, 0));
    qDebug() << "info category map:";
    for (auto [key, value] : info.asKeyValueRange()) {
        series->append(key, value);
        series->setName(key);
        qDebug() << key << value << series->name();
    }
    series->setVerticalPosition(0.17);
    for (auto slice : series->slices()) {
        slice->setLabel(slice->label() + ' ' + QString::number(slice->value()) + " руб.");
    }
    connect(series, SIGNAL(clicked(QPieSlice*)), this, SLOT(showStatistic()));
    chart->addSeries(series);
}

void StatisticsWindow::on_back_clicked() {
    emit openMainWindow();
}
