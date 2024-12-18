#include "sceneellipse.h"

#include "coordinatescene.h"

SceneEllipse::SceneEllipse(QPointF center_, qreal rx, qreal ry, CoordinateScene *scene_)
    : SceneObject(scene_)
{
    this->ellipseCenter = center_;
    this->radiusX = rx;
    this->radiusY = ry;
}

SceneEllipse::SceneEllipse(QPointF center_, qreal r, CoordinateScene *scene_)
    : SceneObject(scene_)
{
    this->ellipseCenter = center_;
    this->radiusX = r;
    this->radiusY = r;
}

QRectF SceneEllipse::boundingRect() const
{
    return QRectF(this->ellipseCenter.x(), this->ellipseCenter.y(), this->radiusX, this->radiusY);
}

void SceneEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF boundingRect = this->boundingRect();
    if (!this->scene->getGraphicsWindow().intersects(boundingRect)) {
        return;
    }
    painter->setTransform(this->scene->getTransform());
    painter->setPen(Qt::magenta);

    this->scene->drawEllipse(painter, this->boundingRect());
}
