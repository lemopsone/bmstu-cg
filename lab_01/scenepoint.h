#ifndef SCENEPOINT_H
#define SCENEPOINT_H

#include <QPainter>
#include <QGraphicsItem>
#include "coordinatescene.h"

class ScenePoint : public QGraphicsItem
{
public:
    ScenePoint(QPointF coords_, CoordinateScene *scene_);
    ~ScenePoint();
    // overrides
    QRectF boundingRect() const override;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    CoordinateScene *scene;
    QPointF coords;
};

#endif // SCENEPOINT_H
