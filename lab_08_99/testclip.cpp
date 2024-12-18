#include "testclip.h"

TestClip::TestClip(QObject *parent) : QObject(parent) {}

void TestClip::testIntersection() {
  Clipper clipper;
  QLine line1(0, 0, 10, 10);
  QLine line2(5, 5, 15, 15);
  QLine line3(0, 0, 5, 5);

  QPolygon shape;
  shape
    << QPoint(0, 0)
    << QPoint(10, 0)
    << QPoint(10, 10)
    << QPoint(0, 10);
  clipper.setShape(shape);
  clipper.setLocked(true);
  QCOMPARE_NE(clipper.clip(line1), QLine());
  QCOMPARE_NE(clipper.clip(line2), QLine());
  QCOMPARE_NE(clipper.clip(line3), QLine());
}

void TestClip::testNoIntersection() {
  Clipper clipper;
  QLine line1(0, 0, 10, 10);
  QLine line2(10, 10, 15, 15);
  QLine line3(0, 0, 5, 5);
  QLine line4(5, 5, 10, 10);
  ClipShape shape;
  shape
    << QPoint(100, 205)
    << QPoint(105, 200)
    << QPoint(115, 208)
    << QPoint(110, 215)
    << QPoint(10, 215);
  clipper.setShape(shape);
  clipper.setLocked(true);

  QCOMPARE(clipper.clip(line1), QLine());
  QCOMPARE(clipper.clip(line2), QLine());
  QCOMPARE(clipper.clip(line3), QLine());
  QCOMPARE(clipper.clip(line4), QLine());
}

QTEST_MAIN(TestClip);
