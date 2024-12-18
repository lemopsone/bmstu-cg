#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QGraphicsItem>
#include <QPainter>

#include "actions.h"

class CoordinateScene;

class SceneObject : public QGraphicsItem
{
public:
    SceneObject(CoordinateScene *scene);

    virtual QRectF boundingRect() const override = 0;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override
        = 0;

    virtual void move(Move move) = 0;
    virtual void rotate(Rotate rotate) = 0;
    virtual void scale(Scale scale) = 0;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    CoordinateScene *scene;
};

#endif // SCENEOBJECT_H
