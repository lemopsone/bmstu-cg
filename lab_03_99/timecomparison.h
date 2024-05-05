#ifndef TIMECOMPARISON_H
#define TIMECOMPARISON_H

#include <QWidget>
#include <QtCharts>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>


namespace Ui {
class TimeComparison;
}

class TimeComparison : public QChartView
{
    Q_OBJECT

public:
    explicit TimeComparison(QWidget *parent = nullptr);
    ~TimeComparison();

private:
    Ui::TimeComparison *ui;
    QBarSeries *createSeries();
    void measureTime(QBarSet *setDDA,
                     QBarSet *setBresenhamInt,
                     QBarSet *setBresenhamFloat,
                     QBarSet *setBresenhamSmooth,
                     QBarSet *setWu);
};

#endif // TIMECOMPARISON_H
