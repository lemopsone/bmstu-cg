#include "testclip.h"

TestClip::TestClip(QObject *parent) : QObject(parent) {}

void TestClip::testIntersection() {
  Clipper clipper;
  QLine line1(0, 0, 10, 10);
  QLine line2(5, 5, 15, 15);
  QLine line3(0, 0, 5, 5);
  ClipShape shape(0, 0, 10, 10);
  clipper.setShape(shape);

  QCOMPARE(clipper.clip(line1), QLine(10, 10, 0, 0));
  QCOMPARE(clipper.clip(line2), QLine(10, 10, 5, 5));
  QCOMPARE(clipper.clip(line3), QLine(0, 0, 5, 5));
}

void TestClip::testNoIntersection() {
  Clipper clipper;
  QLine line1(0, 0, 10, 10);
  QLine line2(10, 10, 15, 15);
  QLine line3(0, 0, 5, 5);
  QLine line4(5, 5, 10, 10);
  ClipShape shape(100, 100, 10, 10);
  clipper.setShape(shape);

  QCOMPARE(clipper.clip(line1), QLine());
  QCOMPARE(clipper.clip(line2), QLine());
  QCOMPARE(clipper.clip(line3), QLine());
  QCOMPARE(clipper.clip(line4), QLine());
}

QTEST_MAIN(TestClip);
