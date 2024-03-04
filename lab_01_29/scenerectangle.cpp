#include "scenerectangle.h"

SceneRectangle::SceneRectangle(MyRectF rect_, CoordinateScene *scene_) : SceneObject(scene_)
{
    this->rect = rect_;
}

QRectF SceneRectangle::boundingRect() const
{
    return this->rect.boundingRect();
}

void SceneRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF boundingRect = this->boundingRect();
    if (!this->scene->getGraphicsWindow().intersects(boundingRect))
    {
        return;
    }
    painter->setTransform(this->scene->getTransform());
    QPen pen = QPen(QColor(QString("#FC6600")));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->setBrush(QColor(QString("#FC6600")));

    this->scene->drawMyRect(painter, rect);
}
