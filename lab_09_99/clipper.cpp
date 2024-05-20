#include "clipper.h"

#include <QDebug>
Clipper::Clipper() {}

Clipper::Clipper(ClipShape shape) {
  setShape(shape);
  createEdges();
}

ClipShape Clipper::shape() const { return shape_; }

bool Clipper::locked() { return locked_; }

void Clipper::setLocked(bool locked) { locked_ = locked; }

void Clipper::setShape(ClipShape shape) {
  shape_ = shape;
  createEdges();
}

void Clipper::addPoint(QPoint point) {
  shape_ << point;
  createEdges();
}

void Clipper::createEdges() {
  edges.clear();
  for (int i = 0; i < shape_.size(); i++) {
    QPoint p1 = shape_.at(i);
    QPoint p2 = shape_.at((i + 1) % shape_.size());
    edges << QLine(p1, p2);
  }
}

bool Clipper::isConvex() {
  QVector2D a(edges.first().p2() - edges.first().p1());
  int sign = 0;
  for (int i = 1; i < edges.size() - 1; i++) {
    QVector2D b(edges.at(i).p2() - edges.at(i).p1());
    double z = a.x() * b.y() - a.y() * b.x();
    if (sign == 0) {
      sign = Clipper::sign(z);
    }
    if (z != 0 && sign != Clipper::sign(z)) {
      sign_ = 0;
      return false;
    }
    a = b;
  }
  sign_ = sign;
  return true;
}

int Clipper::sign(int x) { return (x > 0) - (x < 0); }

int Clipper::visibility(const QPoint &point, const QLine &edge) {
  QVector2D a(edge.p1() - point);
  QVector2D b(edge.p2() - edge.p1());

  double pseudoscalar = a.x() * b.y() - a.y() * b.x();

  return sign(pseudoscalar) * sign_;
}

bool Clipper::intersects(const QPoint &p1, const QPoint &p2,
                         const QLine &edge) {
  return visibility(p1, edge) * visibility(p2, edge) < 0;
}

QList<QPolygon> Clipper::clipMultiple(const QList<QPolygon> &polygons) {
  QList<QPolygon> result;
  for (int i = 0; i < polygons.size(); i++) {
    result << clip(polygons.at(i));
  }
  return result;
}

QPolygon Clipper::clip(const QPolygon &polygon) {
  if (!isConvex() || !locked()) {
    return QPolygon();
  }
  QPolygon result = polygon;
  for (const QLine &edge : edges) {
    result = clipPolygonByEdge(result, edge);
    if (result.isEmpty()) {
      break;
    }
  }
  return result;
}

QPolygon Clipper::clipPolygonByEdge(const QPolygon &polygon,
                                    const QLine &edge) {
  QPolygon result;
  QPoint S, F;

  for (qsizetype j = 0; j < polygon.size(); j++) {
    if (j == 0) {
      F = polygon.at(j);
    } else {
      if (intersects(S, polygon.at(j), edge)) {
        result << intersection(S, polygon.at(j), edge);
      }
    }
    S = polygon.at(j);
    if (visibility(S, edge) >= 0) {
      result << S;
    }
  }
  if (result.isEmpty()) {
    return result;
  }

  if (intersects(S, F, edge)) {
    result << intersection(S, F, edge);
  }
  return result;
}

QPoint Clipper::intersection(const QPoint &p1, const QPoint &p2,
                             const QLine &edge) {
  QPoint q1 = edge.p1(), q2 = edge.p2();

  double dxp = p2.x() - p1.x();
  double dyp = p2.y() - p1.y();
  double dxq = q2.x() - q1.x();
  double dyq = q2.y() - q1.y();
  double t = 0;

  if (dxq != 0) {
    double mq = dyq / dxq;
    t = (q1.y() - p1.y() + mq * (p1.x() - q1.x())) / (dyp - mq * dxp);
  } else
    t = double(q1.x() - p1.x()) / dxp;

  int x = p1.x() + (p2.x() - p1.x()) * t;
  int y = p1.y() + (p2.y() - p1.y()) * t;
  return QPoint(x, y);
}
