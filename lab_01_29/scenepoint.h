#ifndef SCENEPOINT_H
#define SCENEPOINT_H

#include "sceneobject.h"

class ScenePoint : public SceneObject
{
public:
    ScenePoint(QPointF coords_, CoordinateScene *scene_);
    // overrides
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QPointF coords;
};

#endif // SCENEPOINT_H
