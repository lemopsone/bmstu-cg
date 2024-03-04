#include "sceneobject.h"

SceneObject::SceneObject(CoordinateScene *scene_)
{
    this->scene = scene_;
    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
    this->setFlag(QGraphicsItem::ItemIsFocusScope, false);
}

void SceneObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return;
}

void SceneObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    return;
}
