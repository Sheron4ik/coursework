#include "statisticswindow.h"
#include "ui_statisticswindow.h"

StatisticsWindow::StatisticsWindow(Database *DB, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWindow)
{
    ui->setupUi(this);
    DBstatistic = DB; //new Database();

    series = new QPieSeries();
//TODO: legend sizes...
    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignLeft);
    chart->legend()->detachFromChart();
    //chart->legend()->setInteractive(true);
    chart->legend()->setBackgroundVisible(false);
    chart->legend()->setMaximumSize(350, 400);
    chart->legend()->setGeometry(7, 285, 350, 400);
    chart->legend()->update();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setBackgroundVisible(false);

    view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    ui->graph->layout()->addWidget(view);
}

StatisticsWindow::~StatisticsWindow() {
    ui->graph->layout()->removeWidget(view);
    delete view;
    //delete DBstatistic;
    delete ui;
}

QPushButton *StatisticsWindow::getShoppingButton() {
    return ui->back;
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
    series->setVerticalPosition(0.20);
    for (auto slice : series->slices()) {
        slice->setLabel(slice->label() + ' ' + QString::number(slice->value()) + " руб.");
        slice->setBrush(colors[currentColor++]);
        currentColor %= 10;
    }
    connect(series, SIGNAL(clicked(QPieSlice*)), this, SLOT(showCategoryStatistic(QPieSlice*)));
    chart->setTitle("Всего потрачено: " + QString::number(series->sum()) + " руб.");
    chart->addSeries(series);
    this->show();

    /*//view->setRenderHint(QPainter::Antialiasing);
    //view->setAlignment(Qt::AlignBottom);
    //view->setSizePolicy(ui->graph->sizePolicy());
    //view->fitInView(ui->graph->frameRect());
    //view->setParent(ui->graph);*/

    /*qDebug() << "chart " << chart->x() << chart->y() << chart->size() << chart->geometry();
    qDebug() << "chart legend " << chart->legend()->x() << chart->legend()->y() << chart->legend()->size() << chart->legend()->geometry();
    qDebug() << "graph " << ui->graph->x() << ui->graph->y() << ui->graph->size() << ui->graph->geometry();
    qDebug() << "pieSize" << series->pieSize();*/
}

void StatisticsWindow::showCategoryStatistic(QPieSlice* seriesOfCategory) {
    QString nameOfCategory = seriesOfCategory->label().section(' ', 0, 0);
    chart->removeSeries(series);
    delete series;
    series = new QPieSeries();
    QHash<QString, double> info = DBstatistic->getPurchasesForCategoryStatistic(nameOfCategory);
    qDebug() << "info category map:";
    for (auto [key, value] : info.asKeyValueRange()) {
        series->append(key, value);
        series->setName(key);
        qDebug() << key << value << series->name();
    }
    series->setVerticalPosition(0.20);
    for (auto slice : series->slices()) {
        slice->setLabel(slice->label() + ' ' + QString::number(slice->value()) + " руб.");
        slice->setBrush(colors[currentColor++]);
        currentColor %= 10;
    }
    connect(series, SIGNAL(clicked(QPieSlice*)), this, SLOT(showStatistic()));
    chart->setTitle(nameOfCategory + ": " + QString::number(series->sum()) + " руб.");
    chart->addSeries(series);
}

/*void StatisticsWindow::on_back_clicked() {
    emit openMainWindow();
}*/
