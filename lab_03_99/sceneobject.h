#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QGraphicsItem>
#include <QPainter>


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

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    CoordinateScene *scene;
};

#endif // SCENEOBJECT_H
