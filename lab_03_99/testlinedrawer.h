#ifndef TESTLINEDRAWER_H
#define TESTLINEDRAWER_H

#include "linedrawer.h"
#include <QObject>
#include <QTest>

class TestLineDrawer : public QObject {
  Q_OBJECT
public:
  explicit TestLineDrawer(QObject *parent = 0);
private slots:
  void countStepsHorizontalLine();
  void countStepsVerticalLine();
};

#endif // TESTLINEDRAWER_H
