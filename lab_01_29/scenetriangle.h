#ifndef SCENETRIANGLE_H
#define SCENETRIANGLE_H

#include "mytrianglef.h"
#include "sceneobject.h"

class SceneTriangle : public SceneObject
{
public:
    SceneTriangle(MyTriangleF triangle_, CoordinateScene *scene_);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    MyTriangleF triangle;
};

#endif // SCENETRIANGLE_H
