#include "testclip.h"

TestClip::TestClip(QObject *parent) : QObject(parent) {}

void TestClip::testIntersection() {
  Clipper clipper;

  QPolygon userInput;
  userInput
    << QPoint(100, 100)
    << QPoint(150, 150)
    << QPoint(200, 100)
    << QPoint(150, 50);
  QPolygon shape;
  shape
    << QPoint(0, 125)
    << QPoint(250, 125)
    << QPoint(250, 175)
    << QPoint(0, 175);
  clipper.setShape(shape);
  clipper.setLocked(true);
  QCOMPARE_NE(clipper.clip(userInput), QPolygon());
}

void TestClip::testNoIntersection() {
  Clipper clipper;
  QPolygon userInput;
  userInput << QPoint(5, 5)
    << QPoint(15, 5)
    << QPoint(15, 15)
    << QPoint(5, 15)
    << QPoint(5, 50);
  ClipShape shape;
  shape
    << QPoint(100, 205)
    << QPoint(105, 200)
    << QPoint(115, 208)
    << QPoint(110, 215)
    << QPoint(10, 215);
  clipper.setShape(shape);
  clipper.setLocked(true);

  QCOMPARE(clipper.clip(userInput), QPolygon());
}

QTEST_MAIN(TestClip);
