#ifndef _CLIPPER_H
#define _CLIPPER_H

#include <QLine>
#include <QList>
#include <QRect>

enum STATE_CODE { INSIDE = 0, LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8 };

typedef unsigned short StateCode;

typedef QRect ClipShape;

class Clipper {
public:
  Clipper();
  Clipper(ClipShape shape);

  ClipShape shape() const;
  void setShape(ClipShape shape);

  QLine clip(const QLine &line, double eps = std::sqrt(2)) const;
  QList<QLine> clipMultiple(const QList<QLine> &lines,
                            double eps = std::sqrt(2)) const;

  static QPoint midpoint(const QPoint &p1, const QPoint &p2);
  static qreal distance(const QPoint &p1, const QPoint &p2);
  StateCode code(const QPoint &p) const;

private:
  ClipShape shape_;
};

#endif // _CLIPPER_H
