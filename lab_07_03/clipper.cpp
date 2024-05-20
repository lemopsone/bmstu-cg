#include "clipper.h"

Clipper::Clipper() {}

Clipper::Clipper(ClipShape shape) { setShape(shape); }

ClipShape Clipper::shape() const { return shape_; }

void Clipper::setShape(ClipShape shape) { shape_ = shape; }

qreal Clipper::distance(const QPoint &p1, const QPoint &p2) {
  return QLineF(p1, p2).length();
}

StateCode Clipper::code(const QPoint &p) const {
  StateCode code = INSIDE;

  if (p.x() < shape_.left()) {
    code |= LEFT;
  } else if (p.x() > shape_.right()) {
    code |= RIGHT;
  }
  if (p.y() > shape_.bottom()) {
    code |= BOTTOM;
  } else if (p.y() < shape_.top()) {
    code |= TOP;
  }

  return code;
}

QPoint Clipper::midpoint(const QPoint &p1, const QPoint &p2) {
  return QPoint((p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2);
}

QLine Clipper::clip(const QLine &line, double eps) const {
  QLine clippedLine = line;
  QPoint p1 = line.p1(), p2 = line.p2();
  StateCode code1 = code(p1);
  StateCode code2 = code(p2);

  // Полностью видим
  if (code1 == INSIDE && code2 == INSIDE) {
    return clippedLine;
  }

  // Полностью невидим
  if (code1 & code2) {
    return QLine();
  }

  QPoint temp = line.p1();
  StateCode tempCode = code1;
  QList<QPoint> clippedLinePoints;

  // Ищем две вершины видимой части
  for (size_t i = 0; i < 2; ++i) {
    if (!code2) {
      clippedLinePoints << p2;
    } else {
      while (distance(p1, p2) > eps) {
        QPoint mid = midpoint(p1, p2);
        StateCode midCode = code(mid);

        if (midCode & code2) {
          p2 = mid;
          code2 = midCode;
        } else {
          p1 = mid;
          code1 = midCode;
        }
      }

      if (code1 & code2) {
        return QLine();
      }
      clippedLinePoints << p2;
    }

    p1 = p2;
    code1 = code2;
    p2 = temp;
    code2 = tempCode;
  }

  clippedLine.setP1(clippedLinePoints[0]);
  clippedLine.setP2(clippedLinePoints[1]);

  return clippedLine;
}

QList<QLine> Clipper::clipMultiple(const QList<QLine> &lines,
                                   double eps) const {
  QList<QLine> clippedLines;

  for (const QLine &line : lines) {
    clippedLines << clip(line, eps);
  }

  return clippedLines;
}
