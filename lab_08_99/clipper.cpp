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
  qDebug() << "Added point: " << point;
}

void Clipper::createEdges() {
  edges.clear();
  for (int i = 0; i < shape_.size(); i++) {
    QLine line(shape_.at(i), shape_.at((i + 1) % shape_.size()));
    QVector2D edge(line.dx(), line.dy());
    edges << edge;
  }
}

bool Clipper::isConvex() {
  QVector2D prevEdge = edges.first();
  int sign = 0;
  for (qsizetype i = 1; i < edges.size() - 1; ++i) {
    QVector2D edge = edges.at(i);
    int z = prevEdge.x() * edge.y() - prevEdge.y() * edge.x();
    int signZ = Clipper::sign(z);
    if (sign == 0) {
      sign = signZ;
    }
    if (sign != signZ) {
      this->sign_ = 0;
      return false;
    }
    prevEdge = edge;
  }
  this->sign_ = sign;
  return true;
}

int Clipper::sign(int x) { return (x > 0) - (x < 0); }

QLine Clipper::clip(const QLine &line) {
  if (!locked() || !isConvex()) {
    return QLine();
  }
  QPoint p1 = line.p1();

  QVector2D d(line.dx(), line.dy());
  double t1 = 0, t2 = 1;

  for (qsizetype i = 0; i < edges.size(); ++i) {
    QVector2D edge = edges.at(i);
    QVector2D normal;
    if (sign_ == 1) {
      normal = QVector2D(-edge.y(), edge.x());
    } else {
      normal = QVector2D(edge.y(), -edge.x());
    }
    QVector2D w = QVector2D(p1) - QVector2D(shape_.at(i));
    double D = QVector2D::dotProduct(normal, d);
    double W = QVector2D::dotProduct(normal, w);

    if (D == 0) {
      if (W < 0) {
        return QLine();
      }
    } else {
      double t = -W / D;
      if (D > 0) {
        if (t > 1) {
          return QLine();
        } else {
          t1 = std::max(t1, t);
        }
      } else {
        if (t < 0) {
          return QLine();
        } else {
          t2 = std::min(t2, t);
        }
      }
    }
  }

  if (t1 > t2) {
    return QLine();
  } else {
    return QLine(p1 + t1 * d.toPoint(), p1 + t2 * d.toPoint());
  }
}

QList<QLine> Clipper::clipMultiple(const QList<QLine> &lines) {
  QList<QLine> clippedLines;

  for (const QLine &line : lines) {
    clippedLines << clip(line);
  }

  return clippedLines;
}
