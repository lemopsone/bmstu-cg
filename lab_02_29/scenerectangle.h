#ifndef SCENERECTANGLE_H
#define SCENERECTANGLE_H

#include "myrectf.h"
#include "sceneobject.h"

class SceneRectangle : public SceneObject
{
public:
    SceneRectangle(MyRectF rect_, CoordinateScene *scene_);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    MyRectF rect;
    virtual void move(Move move) override;
    virtual void rotate(Rotate rotate) override;
    virtual void scale(Scale scale) override;
};

#endif // SCENERECTANGLE_H
