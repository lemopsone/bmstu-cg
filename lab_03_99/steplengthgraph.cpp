#include "steplengthgraph.h"
#include "ui_steplengthgraph.h"

StepLengthGraph::StepLengthGraph(qsizetype lineLength, QWidget *parent)
    : QWidget(parent), ui(new Ui::StepLengthGraph) {
  ui->setupUi(this);

  QStringList methodNames{"ЦДА", "Брезенхем (целые числа)",
                          "Брезенхем (вещественные числа)",
                          "Брезенхем (устранение ступенчатости)", "Ву"};

  QList<QLineSeries *> individualSeries(5);

  QChart *finalChart = new QChart;

  QPoint p1(0, 0), p2(lineLength, 0);
  for (size_t i = 0; i < 5; i++) {
    QChart *chart = new QChart();

    individualSeries[i] = new QLineSeries;
    individualSeries[i]->setName(methodNames.at(i));

    for (int angle = 0; angle < 91; angle++) {
      QPoint rotatedEnd = rotatePoint(p2, p1, angle);
      QList<Pixel> pixels;
      switch (i) {
      case 0:
        pixels = LineDrawer::DDA(p1, rotatedEnd);
        break;
      case 1:
        pixels = LineDrawer::BresenhamInt(p1, rotatedEnd);
        break;
      case 2:
        pixels = LineDrawer::Bresenham(p1, rotatedEnd);
        break;
      case 3:
        pixels = LineDrawer::BresenhamSmooth(p1, rotatedEnd);
        break;
      case 4:
        pixels = LineDrawer::Wu(p1, rotatedEnd);
        break;
      }
      qsizetype stepCount = LineDrawer::stepCount(pixels);
      individualSeries[i]->append(angle, stepCount);
    }

    individualSeries[i]->setMarkerSize(5);
    individualSeries[i]->setColor(QColor(Qt::GlobalColor(i + 7)));

    chart->addSeries(individualSeries[i]);
    finalChart->addSeries(copySeries(individualSeries[i]));
    chart->createDefaultAxes();
    chart->setTitle(methodNames[i]);
    chart->axes(Qt::Horizontal).at(0)->setTitleText("Угол");
    chart->axes(Qt::Vertical).at(0)->setTitleText("Количество ступенек");
    chart->axes(Qt::Vertical).at(0)->setRange(0, lineLength);
    chart->legend()->hide();

    QChartView *view = new QChartView(chart);
    ui->grid->addWidget(view, (i + 1) / 3, (i + 1) % 3, 1, 1);
  }

  finalChart->createDefaultAxes();
  finalChart->setTitle("Сравнение методов");
  finalChart->axes(Qt::Horizontal).at(0)->setTitleText("Угол");
  finalChart->axes(Qt::Vertical).at(0)->setTitleText("Количество ступенек");
  finalChart->axes(Qt::Vertical).at(0)->setRange(0, lineLength);
  finalChart->legend()->setVisible(true);
  finalChart->legend()->detachFromChart();
  finalChart->legend()->setGeometry(175, 275, 300, 125);
  QChartView *finalView = new QChartView(finalChart);
  ui->grid->addWidget(finalView, 0, 0, 1, 1);

  ui->grid->setSpacing(10);
}

StepLengthGraph::~StepLengthGraph() { delete ui; }

QLineSeries *StepLengthGraph::copySeries(const QLineSeries *prev) {
  QLineSeries *newSeries = new QLineSeries;

  for (auto point : prev->points()) {
    newSeries->append(point);
  }

  newSeries->setName(prev->name());
  newSeries->setMarkerSize(prev->markerSize());
  newSeries->setColor(prev->color());

  return newSeries;
}
