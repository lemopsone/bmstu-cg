#ifndef SCENEELLIPSE_H
#define SCENEELLIPSE_H

#include "sceneobject.h"
#include "myellipsef.h"

class SceneEllipse : public SceneObject
{
public:
    SceneEllipse(MyEllipseF ellipse, CoordinateScene *scene_);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual void move(Move move) override;
    virtual void rotate(Rotate rotate) override;
    virtual void scale(Scale scale) override;
private:
    MyEllipseF _ellipse;
};

#endif // SCENEELLIPSE_H
