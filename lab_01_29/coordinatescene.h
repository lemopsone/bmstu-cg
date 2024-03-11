#ifndef COORDINATESCENE_H
#define COORDINATESCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QObject>
#include "scenepoint.h"
#include "sceneellipse.h"
#include "scenerectangle.h"
#include "sceneline.h"
#include "scenetriangle.h"
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

    void drawPoint(QPainter *painter, QPointF point, QString text = QString());
    void drawLine(QPainter *painter, QLineF line, bool showAngle = false);
    void drawMyRect(QPainter *painter, MyRectF rect);
    void drawMyTriangle(QPainter *painter, MyTriangleF triangle);
    void drawEllipse(QPainter *painter, QRectF ellipse);
    void drawText(QPainter *painter, QPointF startingPos, QString text);
    void drawText(QPainter *painter, QPointF startingPos, QSizeF size, QString text);
    void drawLineAngle(QPainter *painter, QLineF line);

    QList<ScenePoint *> getPoints(void);
    void addPoint(ScenePoint *point);
    void removePoint(ScenePoint *point);
    void removePoint(qsizetype idx);

    QList<SceneRectangle *> getRectangles(void);
    void addRectangle(SceneRectangle *rectangle);
    void removeRectangle(SceneRectangle *rectangle);
    void removeRectangle(qsizetype idx);

    QList<SceneEllipse *> getEllipses(void);
    void addEllipse(SceneEllipse *point);
    void removeEllipse(SceneEllipse *circle);
    void removeEllipse(qsizetype idx);

    QList<SceneLine *> getLines(void);
    void addLine(SceneLine *line);
    void removeLine(SceneLine *line);
    void removeLine(qsizetype idx);

    QList<SceneTriangle *> getTriangles(void);
    void addTriangle(SceneTriangle *triangle);
    void removeTriangle(SceneTriangle *triangle);
    void removeTriangle(qsizetype idx);

    void setTransform(const QTransform transform);
    QTransform getTransform(void) const;

    void setAutoScale(bool state);
    bool getAutoScale(void);
    void autoScaleContents(void);

    void setPointSelection(qsizetype idx, bool state);

    void removeItem(QGraphicsItem *item);
    void addItem(QGraphicsItem *item);
    QRectF findItemsBoundingRect();
signals:
    void zoomChanged(double value);
    void sceneClicked(QPointF point);
    void pointDeleted(ssize_t pointIndex);
    void mousePositionChanged(QPointF currentPos);
    void taskSolutionFound(QRectF circle, QLineF line);
    void itemsChanged(void);

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
    bool autoScale;
    QTransform transform;

    // множество точек
    QList<ScenePoint *> points;

    QList<SceneEllipse *> ellipses;
    QList<SceneLine *> lines;
    QList<SceneRectangle *> rectangles;
    QList<SceneTriangle *> triangles;

    //  Приватные методы
    void computeCoefficients(void);

    void drawXAxis(QPainter *painter);
    void drawYAxis(QPainter *painter);
    void drawCenter(QPainter *painter);

    void removeIntersectingPoint(QPointF point);

    double getDistance(QPointF a, QPointF b);
    QString printableAngle(qreal angle);

};




#endif // COORDINATESCENE_H
