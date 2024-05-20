#ifndef _TESTCLIP_H_
#define _TESTCLIP_H_

#include <QObject>
#include <QTest>
#include "clipper.h"

class TestClip : public QObject {
  Q_OBJECT
public:
  explicit TestClip(QObject *parent = 0);
private slots:
  void testIntersection();
  void testNoIntersection();
};

#endif // _TESTCLIP_H_
