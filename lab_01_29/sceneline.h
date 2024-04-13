#ifndef SCENELINE_H
#define SCENELINE_H

#include "sceneobject.h"

class SceneLine : public SceneObject
{
public:
    SceneLine(QLineF line_, CoordinateScene *scene_);
    // overrides
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QLineF line;
};

#endif // SCENELINE_H
