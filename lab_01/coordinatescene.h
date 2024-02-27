#ifndef COORDINATESCENE_H
#define COORDINATESCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QObject>

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
    QList<QGraphicsItem *> points;

    // this or
    QList<QGraphicsItem *> circles;
    QList<QGraphicsItem *> lines;
    QList<QGraphicsItem *> rectangles;
    // this
    QList<QGraphicsItem *> solution;

    //  Приватные методы
    void computeCoefficients(void);

    void drawXAxis(QPainter *painter);
    void drawYAxis(QPainter *painter);
    void drawCenter(QPainter *painter);

    void removeIntersectingPoint(QPointF point);

    double getDistance(QPointF a, QPointF b);

};




#endif // COORDINATESCENE_H
