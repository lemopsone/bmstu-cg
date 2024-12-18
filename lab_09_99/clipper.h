#ifndef _CLIPPER_H
#define _CLIPPER_H

#include <QLine>
#include <QList>
#include <QPolygon>
#include <QVector2D>
#include <cmath>

typedef QPolygon ClipShape;

class Clipper {
public:
  Clipper();
  Clipper(ClipShape shape);

  ClipShape shape() const;
  void setShape(ClipShape shape);
  void addPoint(QPoint point);
  bool locked();
  void setLocked(bool locked);

  QPolygon clip(const QPolygon &polygon);
  QPolygon clipPolygonByEdge(const QPolygon &polygon, const QLine &edge);
  QList<QPolygon> clipMultiple(const QList<QPolygon> &polygons);

private:
  ClipShape shape_;
  int sign_ = -1;

  int visibility(const QPoint &point, const QLine &edge);
  bool intersects(const QPoint &p1, const QPoint &p2, const QLine &edge);
  QPoint intersection(const QPoint &p1, const QPoint &p2, const QLine &edge);

  bool isConvex();
  void createEdges();
  QList<QLine> edges;

  bool locked_ = false;
  static int sign(int x);
};

#endif // _CLIPPER_H
