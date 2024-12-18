#ifndef _TESTFILL_H_
#define _TESTFILL_H_

#include <QObject>
#include <QTest>
#include "fill.h"

class TestFill : public QObject {
  Q_OBJECT
public:
  explicit TestFill(QObject *parent = 0);
private slots:
  void testSquare();
};

#endif // _TESTFILL_H_
