#include "scenetriangle.h"

SceneTriangle::SceneTriangle(MyTriangleF triangle_, CoordinateScene *scene_) : SceneObject(scene_)
{
    this->triangle = triangle_;
}

QRectF SceneTriangle::boundingRect() const
{
    return this->triangle.boundingRect();
}

void SceneTriangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF boundingRect = this->boundingRect();
    if (!this->scene->getGraphicsWindow().intersects(boundingRect))
    {
        return;
    }
    painter->setTransform(this->scene->getTransform());
    painter->setPen(Qt::black);
    painter->setBrush(QColor(Qt::cyan));

    this->scene->drawMyTriangle(painter, this->triangle);
}
