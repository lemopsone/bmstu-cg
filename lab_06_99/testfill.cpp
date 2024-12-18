#include "testfill.h"

TestFill::TestFill(QObject *parent) : QObject(parent) {}

void TestFill::testSquare()
{
  QList<QPoint> points;
  points << QPoint(0, 0) << QPoint(0, 100) << QPoint(100, 100) << QPoint(100, 0);
  CoordinateScene scene(QRectF(0, 0, 1024, 800));
  FillColors colors;
  Q_ASSERT(fill(&scene, colors, QPoint(50, 50), 0) > 0);
}

QTEST_MAIN(TestFill);
