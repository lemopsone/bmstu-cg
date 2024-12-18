#ifndef _CLIPPER_H
#define _CLIPPER_H

#include <QLine>
#include <QList>
#include <QPolygon>
#include <cmath>
#include <QVector2D>

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

  QLine clip(const QLine &line);
  QList<QLine> clipMultiple(const QList<QLine> &lines);

private:
  ClipShape shape_;
  int sign_ = -1;

  bool isConvex();
  void createEdges();
  QList<QVector2D> edges;

  bool locked_ = false;
  static int sign(int x);
};

#endif // _CLIPPER_H
