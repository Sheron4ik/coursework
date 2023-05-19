#include "statisticswindow.h"
#include "ui_statisticswindow.h"

StatisticsWindow::StatisticsWindow(Database *DB, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticsWindow)
{
    ui->setupUi(this);
    DBstatistic = DB;
    date = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1);

    series = new QPieSeries();

    chart = new QChart();
    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignLeft);
    chart->legend()->detachFromChart();
    chart->legend()->setBackgroundVisible(false);
    chart->legend()->setMaximumSize(1000, 1000);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setBackgroundVisible(false);

    view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    ui->graph->layout()->addWidget(view);
}

StatisticsWindow::~StatisticsWindow() {
    ui->graph->layout()->removeWidget(view);
    delete view;
    delete ui;
}

QPushButton *StatisticsWindow::getShoppingButton() {
    return ui->back;
}

void StatisticsWindow::showStatistic() {
    chart->removeSeries(series);
    delete series;
    series = new QPieSeries();
    QList<QPair<QString, double>> info = DBstatistic->getPurchasesForStatistics(date.toString("yyyy.MM.dd"));
    for (const auto &data : info) {
        series->append(data.first, data.second);
        series->setName(data.first);
    }
    series->setVerticalPosition(0.2);
    for (auto slice : series->slices()) {
        slice->setLabel(slice->label() + ' ' + QString::number(slice->value()) + " руб.");
        slice->setBrush(colors[currentColor++]);
        currentColor %= 10;
    }
    connect(series, SIGNAL(clicked(QPieSlice*)), this, SLOT(showCategoryStatistic(QPieSlice*)));
    chart->setTitle("Всего потрачено: " + QString::number(series->sum()) + " руб.");
    chart->addSeries(series);
    this->show();

    chart->legend()->setGeometry(7, chart->size().width()*series->pieSize()+ui->graph->y(),
                                 chart->size().width()-13, chart->size().height()-(chart->size().width()*series->pieSize()+ui->graph->y())-7);
    chart->legend()->update();
}

void StatisticsWindow::showCategoryStatistic(QPieSlice* seriesOfCategory) {
    QString nameOfCategory = seriesOfCategory->label().section(' ', 0, 0);
    chart->removeSeries(series);
    delete series;
    series = new QPieSeries();
    QList<QPair<QString, double>> info = DBstatistic->getPurchasesForCategoryStatistic(nameOfCategory, date.toString("yyyy.MM.dd"));
    for (const auto &data : info) {
        series->append(data.first, data.second);
        series->setName(data.first);
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

void StatisticsWindow::on_settings_currentTextChanged(const QString& setting) {
    if (setting == "за день") {
        date = QDate::currentDate();
    } else if (setting == "за месяц") {
        date = QDate(QDate::currentDate().year(), QDate::currentDate().month(), 1);
    } else {
        date = QDate(1900, 1, 1);
    }
    showStatistic();
}
