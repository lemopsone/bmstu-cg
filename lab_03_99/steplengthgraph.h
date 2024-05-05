#ifndef STEPLENGTHGRAPH_H
#define STEPLENGTHGRAPH_H

#include <QWidget>
#include <QtCharts>

#include "linedrawer.h"
#include "operations.h"

namespace Ui {
class StepLengthGraph;
}

class StepLengthGraph : public QWidget
{
    Q_OBJECT

public:
    explicit StepLengthGraph(qsizetype lineLength = 100, QWidget *parent = nullptr);
    ~StepLengthGraph();

private:
    Ui::StepLengthGraph *ui;
    QLineSeries *copySeries(const QLineSeries *prev);
};

#endif // STEPLENGTHGRAPH_H
