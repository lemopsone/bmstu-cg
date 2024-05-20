#ifndef COORDINATESCENE_H
#define COORDINATESCENE_H

#include "clipper.h"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QPainter>
#include <qimage.h>

enum DrawMode { POLYGON, CLIPPER };
enum FigurePoint { FIRST, NEXT };

struct SceneColors {
  QColor bgColor = Qt::white;
  QColor polygonColor = Qt::black;
  QColor clipColor = Qt::red;
  QColor clipShapeColor = Qt::blue;
};

class CoordinateScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit CoordinateScene(QRectF sceneRect, SceneColors colors,
                           QObject *parent = nullptr);
  ~CoordinateScene();

  void createPolygon(QPolygon polygon);
  void createClipShape(ClipShape shape);
  void clearScene();

  void repaintPolygons();

  QImage *getImage();
  void setImage(QImage *image);

  void drawImage(QImage *image = nullptr);

  void setDrawMode(DrawMode mode);
  DrawMode getDrawMode();

  void drawPolygon(QPolygon, QPainter *painter,
                bool clipped = false);
  void drawClipShape(ClipShape shape, QPainter *painter, bool closed = false);

  void clipPolygons();

  void setBackgroundColor(QColor color);
  void setPolygonColor(QColor color);
  void setClipColor(QColor color);
  void setClipShapeColor(QColor color);

  void addClipShapePoint(QPoint point);
  void lockClipShape();

  void addPolygonPoint(QPoint point);
  void lockPolygon();

signals:
  void mousePositionChanged(QPointF currentPos);
  void polygonCreated(QPolygon polygon);
  void clipShapeCleared();
  void clipShapePointAdded(QPoint point);
  void clipShapeLockToggled(bool locked);
  void polygonPointAdded(QPoint point);
  void polygonLocked();

protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  virtual void keyPressEvent(QKeyEvent *keyEvent) override;
  virtual void keyReleaseEvent(QKeyEvent *keyEvent) override;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

  QPoint normalizePoint(QPoint point);

  // Аттрибуты
  QColor color = Qt::white;
  QColor polygonColor = Qt::black;
  QColor clipColor = Qt::red;
  QColor clipShapeColor = Qt::blue;
  QImage *image;
  QPainter *painter;
  bool parallelMode = false;
  DrawMode drawMode = POLYGON;
  FigurePoint figurePoint = FIRST;
  QPoint prevPoint;
  QList<QPolygon> polygons;
  QPolygon currentPolygon;
  Clipper clipper;
};

#endif // COORDINATESCENE_H
