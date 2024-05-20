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

enum DrawMode { LINE, CLIPPER };
enum FigurePoint { FIRST, SECOND };

struct SceneColors {
  QColor bgColor = Qt::white;
  QColor lineColor = Qt::black;
  QColor clipColor = Qt::red;
  QColor clipShapeColor = Qt::blue;
};

class CoordinateScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit CoordinateScene(QRectF sceneRect, SceneColors colors,
                           QObject *parent = nullptr);
  ~CoordinateScene();

  void createLine(QPoint start, QPoint end);
  void createClipShape(QPoint start, QPoint end);
  void clearScene();

  void repaintLines();

  QImage *getImage();
  void setImage(QImage *image);

  void drawImage(QImage *image = nullptr);

  void setDrawMode(DrawMode mode);
  DrawMode getDrawMode();

  void drawLine(QPoint start, QPoint end, QPainter *painter,
                bool clipped = false);
  void drawClipShape(QPoint start, QPoint end, QPainter *painter);

  void clipLines();

  void setBackgroundColor(QColor color);
  void setLineColor(QColor color);
  void setClipColor(QColor color);
  void setClipShapeColor(QColor color);

signals:
  void mousePositionChanged(QPointF currentPos);
  void lineCreated(QPoint start, QPoint end);

protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  virtual void keyPressEvent(QKeyEvent *keyEvent) override;
  virtual void keyReleaseEvent(QKeyEvent *keyEvent) override;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

  QPoint normalizePoint(QPoint point);

  // Аттрибуты
  QColor color = Qt::white;
  QColor lineColor = Qt::black;
  QColor clipColor = Qt::red;
  QColor clipShapeColor = Qt::blue;
  QImage *image;
  QPainter *painter;
  bool parallelMode = false;
  DrawMode drawMode = LINE;
  FigurePoint figurePoint = FIRST;
  QPoint startDrawPoint;
  QList<QLine> lines;
  Clipper clipper;
};

#endif // COORDINATESCENE_H
