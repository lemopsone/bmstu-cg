#ifndef COORDINATESCENE_H
#define COORDINATESCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QObject>
#include "myrectf.h"
#include "mytrianglef.h"

class CoordinateScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CoordinateScene(QRectF windowRect, QRectF planeRect, bool state, QObject *parent = nullptr);

    void setGridVisibility(bool value);
    bool gridIsVisible(void);

    void removeAllPoints(void);

    void setPlane(double x, double y, double w, double h, bool update = true);
    void setPlane(QPointF topLeft, double w, double h, bool update = true);
    void setPlane(QRectF rect, bool update = true);
    QRectF getPlane(void);

    void setGraphicsWindow(double x, double y, double w, double h, bool update = true);
    void setGraphicsWindow(QRectF rect, bool update = true);
    QRectF getGraphicsWindow(void);

    void setCX(double c);
    void setCY(double c);
    double getCX(void);
    double getCY(void);

    QPointF toPlaneCoords(QPointF windowCoords);
    QPointF toWindowCoords(QPointF planeCoords);

    void zoomAtPercent(double zoom);

    void drawPoint(QPainter *painter, QPointF point);
    void drawLine(QPainter *painter, QLineF line);
    void drawMyRect(QPainter *painter, MyRectF rect);
    void drawMyTriangle(QPainter *painter, MyTriangleF triangle);
    void drawEllipse(QPainter *painter, QRectF ellipse);

    void addPoint(QGraphicsItem *point);
    void removePoint(QGraphicsItem *point);
    void removePoint(qsizetype idx);

    void addRectangle(QGraphicsItem *rectangle);
    void removeRectangle(QGraphicsItem *rectangle);
    void removeRectangle(qsizetype idx);

    void addEllipse(QGraphicsItem *point);
    void removeEllipse(QGraphicsItem *circle);
    void removeEllipse(qsizetype idx);

    void addLine(QGraphicsItem *line);
    void removeLine(QGraphicsItem *line);
    void removeLine(qsizetype idx);

    void addTriangle(QGraphicsItem *triangle);
    void removeTriangle(QGraphicsItem *triangle);
    void removeTriangle(qsizetype idx);

    void setTransform(const QTransform transform);
    QTransform getTransform(void) const;
signals:
    void zoomChanged(double value);
    void sceneClicked(QPointF point);
    void pointDeleted(ssize_t pointIndex);
    void mousePositionChanged(QPointF currentPos);

protected:
    // Переопределения методов QGraphicsScene
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
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
    QTransform transform;

    // множество точек
    QList<QGraphicsItem *> points;

    QList<QGraphicsItem *> ellipses;
    QList<QGraphicsItem *> lines;
    QList<QGraphicsItem *> rectangles;
    QList<QGraphicsItem *> triangles;

    //  Приватные методы
    void computeCoefficients(void);

    void drawXAxis(QPainter *painter);
    void drawYAxis(QPainter *painter);
    void drawCenter(QPainter *painter);

    void removeIntersectingPoint(QPointF point);

    double getDistance(QPointF a, QPointF b);

};




#endif // COORDINATESCENE_H
