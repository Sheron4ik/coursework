#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QWidget>

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

private slots:
    void on_back_clicked();

private:
    Ui::StatisticsWindow *ui;
};

#endif // STATISTICSWINDOW_H
