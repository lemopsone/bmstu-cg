#include "operations.h"

QPoint rotatePoint(QPoint p, QPoint origin, qreal degree) {
  qreal x, y, cosine, sine;
  QPoint point = p;
  qreal angle = qDegreesToRadians(degree);
  sine = std::sin(angle);
  cosine = std::cos(angle);
  x = origin.x() + (point.x() - origin.x()) * cosine +
      (point.y() - origin.y()) * sine;
  y = origin.y() + (point.y() - origin.y()) * cosine -
      (point.x() - origin.x()) * sine;
  point.setX(x);
  point.setY(y);

  return point;
}
