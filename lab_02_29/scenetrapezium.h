#ifndef SCENETRAPEZIUM_H
#define SCENETRAPEZIUM_H

#include "sceneobject.h"
#include "mytrapeziumf.h"

class SceneTrapezium : public SceneObject
{
public:
    SceneTrapezium(MyTrapeziumF trapezium_, CoordinateScene *scene);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    MyTrapeziumF trapezium;
    virtual void move(Move move) override;
    virtual void rotate(Rotate rotate) override;
    virtual void scale(Scale scale) override;
};

#endif // SCENETRAPEZIUM_H
