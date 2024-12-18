#ifndef _CIRCLEDRAWER_H_
#define _CIRCLEDRAWER_H_

#include "shapeutils.h"
#include <QList>

class CircleDrawer {
public:
  static QList<Pixel> Canonical(const CircleData &data);
  static QList<Pixel> Parametric(const CircleData &data);
  static QList<Pixel> Bresenham(const CircleData &data);
  static QList<Pixel> MiddlePoint(const CircleData &data);
};

#endif // _CIRCLEDRAWER_H_
