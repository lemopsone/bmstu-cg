#ifndef _TESTDRAWING_H_
#define _TESTDRAWING_H_

#include "circledrawer.h"
#include "ellipsedrawer.h"
#include <QObject>
#include <QTest>

class TestDrawing : public QObject {
  Q_OBJECT
public:
  explicit TestDrawing(QObject *parent = 0);
private slots:
  void testEllipseDrawer();
  void testCircleDrawer();
};

#endif // _TESTDRAWING_H_
