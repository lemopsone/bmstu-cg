#ifndef COORDINATESCENE_H
#define COORDINATESCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QPainter>
#include <QElapsedTimer>
#include <qimage.h>
#include "fill.h"


class CoordinateScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit CoordinateScene(QRectF sceneRect,
                           QColor fillColor = Qt::black, QObject *parent = nullptr);
  ~CoordinateScene();

  void removeAllPoints(void);

  QColor getColor() const;
  void setColor(QColor newColor);

  void addPolygonPoint(QPoint point);
  bool isLocked();
  void setLocked(bool state);
  void clearScene();
  qint64 createFilledPolygon();

  int getDelay();
  void setDelay(int delay);

  QImage getPolygonImage();
  void setPolygonImage(QImage image);

signals:
  void sceneClicked(QPoint point);
  void mousePositionChanged(QPointF currentPos);
  void figureLocked();

protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  virtual void keyPressEvent(QKeyEvent *keyEvent) override;
  virtual void keyReleaseEvent(QKeyEvent *keyEvent) override;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

  void drawImage(QImage *image = nullptr);
  QPoint normalizePoint(QPoint point);

  // Аттрибуты
  QColor color = Qt::white;
  QColor fillColor = Qt::black;
  QList<QPoint> polygonPoints;
  bool polygonLocked = false;
  QImage *image;
  QPainter *painter;
  bool parallelMode = false;
  int delay = 0;
};

#endif // COORDINATESCENE_H
