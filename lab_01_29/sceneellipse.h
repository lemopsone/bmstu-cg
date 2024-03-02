#ifndef SCENEELLIPSE_H
#define SCENEELLIPSE_H

#include "sceneobject.h"

class SceneEllipse : public SceneObject
{
public:
    SceneEllipse(QPointF center_, qreal rx, qreal ry, CoordinateScene *scene_);
    SceneEllipse(QPointF center_, qreal r, CoordinateScene *scene_);
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QPointF ellipseCenter;
    qreal radiusX, radiusY;
};

#endif // SCENEELLIPSE_H
