#ifndef COORDINATESCENE_H
#define COORDINATESCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPainter>

#include "linedrawer.h"
#include "operations.h"

class CoordinateScene : public QGraphicsScene {
  Q_OBJECT
public:
  explicit CoordinateScene(QRectF windowRect, QRectF planeRect, bool state,
                           QColor color = Qt::white, QObject *parent = nullptr);

  void setGridVisibility(bool value);
  bool gridIsVisible(void);

  void removeAllPoints(void);

  QColor getColor() const;
  void setColor(QColor newColor);

  void addLine(LineData dataColor);
  void addSpectre(LineData data, qreal step);
  void drawPixels(QPainter *painter, QList<Pixel> pixels);
  void setPlane(double x, double y, double w, double h, bool update = true);
  void setPlane(QPointF topLeft, double w, double h, bool update = true);
  void setPlane(QRectF rect, bool update = true);
  QRectF getPlane(void);

  void setGraphicsWindow(double x, double y, double w, double h,
                         bool update = true);
  void setGraphicsWindow(QRectF rect, bool update = true);
  QRectF getGraphicsWindow(void);

  void setCX(double c);
  void setCY(double c);
  double getCX(void);
  double getCY(void);

  QPointF toPlaneCoords(QPointF windowCoords);
  QPointF toWindowCoords(QPointF planeCoords);

  void zoomAtPercent(double zoom);

  void drawPoint(QPainter *painter, QPointF point, QString text = QString());
  void drawText(QPainter *painter, QPointF startingPos, QString text);
  void drawText(QPainter *painter, QPointF startingPos, QSizeF size,
                QString text);

  void setTransform(const QTransform transform);
  QTransform getTransform(void) const;

  void setAutoScale(bool state);
  bool getAutoScale(void);
  void autoScaleContents(void);

  void setPointSelection(qsizetype idx, bool state);

  QRectF findItemsBoundingRect();
signals:
  void zoomChanged(double value);
  void sceneClicked(QPointF point);
  void pointDeleted(ssize_t pointIndex);
  void mousePositionChanged(QPointF currentPos);

protected:
  // Переопределения методов QGraphicsScene
  virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
  virtual void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) override;

  // Аттрибуты
  QRectF plane, graphicsWindow;
  bool gridVisible;
  double cX, cY;
  QPointF dragStartPos;
  bool isDragged = false;
  QSizeF originalSize;
  double zoomPercent;
  bool autoScale;
  QTransform transform;
  QColor color;

  //  Приватные методы
  void computeCoefficients(void);

  void drawXAxis(QPainter *painter);
  void drawYAxis(QPainter *painter);
  void drawCenter(QPainter *painter);

  double getDistance(QPointF a, QPointF b);
  QString printableAngle(qreal angle);
};

#endif // COORDINATESCENE_H
