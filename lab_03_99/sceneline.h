#ifndef SCENELINE_H
#define SCENELINE_H

#include "sceneobject.h"
#include "linedrawer.h"

class SceneLine : public SceneObject
{
public:
    SceneLine(QLine line, LineType type, QColor color, CoordinateScene *scene_);
    // overrides
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QLine line_;
    LineType type_;
    QColor color_;
};

#endif // SCENELINE_H
