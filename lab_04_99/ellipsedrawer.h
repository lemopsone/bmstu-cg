#ifndef _ELLIPSEDRAWER_H_
#define _ELLIPSEDRAWER_H_

#include "shapeutils.h"
#include <QList>

class EllipseDrawer {
public:
  static QList<Pixel> Canonical(const EllipseData &data);
  static QList<Pixel> Parametric(const EllipseData &data);
  static QList<Pixel> Bresenham(const EllipseData &data);
  static QList<Pixel> MiddlePoint(const EllipseData &data);
};

#endif // _ELLIPSEDRAWER_H_
