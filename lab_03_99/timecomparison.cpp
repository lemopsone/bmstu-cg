#include "timecomparison.h"
#include "ui_timecomparison.h"

#include "linedrawer.h"

TimeComparison::TimeComparison(QWidget *parent)
    : QChartView(parent), ui(new Ui::TimeComparison) {
  ui->setupUi(this);

  auto chart = new QChart;
  auto series = createSeries();
  chart->addSeries(series);
  chart->setTitle("Время выполнения алгоритмов, мкс");

  auto axisY = new QValueAxis;
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  chart->legend()->setAlignment(Qt::AlignBottom);

  setChart(chart);
}

TimeComparison::~TimeComparison() { delete ui; }

QBarSeries *TimeComparison::createSeries() {
  auto setDDA = new QBarSet("DDA");
  auto setBresenhamInt = new QBarSet("Bresenham (int)");
  auto setBresenhamFloat = new QBarSet("Bresenham (float)");
  auto setBresenhamSmooth = new QBarSet("Bresenham (smooth)");
  auto setWu = new QBarSet("Wu");

  QBarSeries *series = new QBarSeries;
  series->append(setDDA);
  series->append(setBresenhamInt);
  series->append(setBresenhamFloat);
  series->append(setBresenhamSmooth);
  series->append(setWu);

  measureTime(setDDA, setBresenhamInt, setBresenhamFloat, setBresenhamSmooth,
              setWu);

  return series;
}

void TimeComparison::measureTime(QBarSet *setDDA, QBarSet *setBresenhamInt,
                                 QBarSet *setBresenhamFloat,
                                 QBarSet *setBresenhamSmooth, QBarSet *setWu) {
  QPoint p1(0, 0), p2(100, 0);

  auto start = std::chrono::high_resolution_clock::now();
  LineDrawer::DDA(p1, p2);
  auto end = std::chrono::high_resolution_clock::now();
  *setDDA << std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                 .count();

  start = std::chrono::high_resolution_clock::now();
  LineDrawer::BresenhamInt(p1, p2);
  end = std::chrono::high_resolution_clock::now();
  *setBresenhamInt << std::chrono::duration_cast<std::chrono::microseconds>(
                          end - start)
                          .count();

  start = std::chrono::high_resolution_clock::now();
  LineDrawer::Bresenham(p1, p2);
  end = std::chrono::high_resolution_clock::now();
  *setBresenhamFloat << std::chrono::duration_cast<std::chrono::microseconds>(
                            end - start)
                            .count();

  start = std::chrono::high_resolution_clock::now();
  LineDrawer::BresenhamSmooth(p1, p2);
  end = std::chrono::high_resolution_clock::now();
  *setBresenhamSmooth << std::chrono::duration_cast<std::chrono::microseconds>(
                             end - start)
                             .count();

  start = std::chrono::high_resolution_clock::now();
  LineDrawer::Wu(p1, p2);
  end = std::chrono::high_resolution_clock::now();
  *setWu << std::chrono::duration_cast<std::chrono::microseconds>(end - start)
                .count();
}
