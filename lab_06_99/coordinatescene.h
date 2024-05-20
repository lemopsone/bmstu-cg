#ifndef COORDINATESCENE_H
#define COORDINATESCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QObject>
#include <QPainter>
#include <qimage.h>


class CoordinateScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit CoordinateScene(QRectF sceneRect,
                           QColor fillColor = Qt::black, QObject *parent = nullptr);
  ~CoordinateScene();

  void removeAllPoints(void);

  QColor getEdgeColor() const;
  void setEdgeColor(QColor newColor);

  void addPolygonPoint(QPoint point);
  bool isLocked();
  void setLocked(bool state);
  void clearScene();

  QImage *getImage();
  void setImage(QImage *image);

  void drawImage(QImage *image = nullptr);
signals:
  void sceneClicked(QPoint point);
  void mousePositionChanged(QPointF currentPos);
  void figureLocked();
  void seedPointSet(QPoint seed);

protected:
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  virtual void keyPressEvent(QKeyEvent *keyEvent) override;
  virtual void keyReleaseEvent(QKeyEvent *keyEvent) override;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

  QPoint normalizePoint(QPoint point);

  // Аттрибуты
  QColor color = Qt::white;
  QColor edgeColor = Qt::black;
  QList<QPoint> polygonPoints;
  bool polygonLocked = false;
  QImage *image;
  QPainter *painter;
  bool parallelMode = false;
};

#endif // COORDINATESCENE_H
