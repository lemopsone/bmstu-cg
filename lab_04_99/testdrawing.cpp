#include "testdrawing.h"

TestDrawing::TestDrawing(QObject *parent) : QObject(parent) {}

void TestDrawing::testEllipseDrawer() {
  EllipseData ellipseData = {QPoint(0, 0), 100, 50};
  QList<Pixel> pixels = EllipseDrawer::Bresenham(ellipseData);
  Q_ASSERT(pixels.isEmpty() == false);
}

void TestDrawing::testCircleDrawer() {
  CircleData circleData = {QPoint(0, 0), 100};
  QList<Pixel> pixels = CircleDrawer::Bresenham(circleData);
  Q_ASSERT(pixels.isEmpty() == false);
}

QTEST_MAIN(TestDrawing);
