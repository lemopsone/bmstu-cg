#include "testfill.h"

TestFill::TestFill(QObject *parent) : QObject(parent) {}

void TestFill::testSquare()
{
  QList<QPoint> points;
  points << QPoint(0, 0) << QPoint(0, 100) << QPoint(100, 100) << QPoint(100, 0);
  QList<PixelRow> pixels = Fill::create(points);
  Q_ASSERT(pixels.size() == 100); /* 100 rows */
}

QTEST_MAIN(TestFill);
