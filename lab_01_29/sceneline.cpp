#include "sceneline.h"
#include "coordinatescene.h"

SceneLine::SceneLine(QLineF line_, CoordinateScene *scene_) : SceneObject(scene_)
{
    this->line = line_;
    this->setPos(this->line.center());
}

QRectF SceneLine::boundingRect() const
{
    return QRectF(this->line.p1(), this->line.p2()).normalized();
}

void SceneLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!this->scene->getGraphicsWindow().intersects(this->boundingRect()))
    {
        return;
    }
    painter->setTransform(this->scene->getTransform());
    QPen pen = QPen(Qt::darkBlue);
    pen.setWidth(2);
    painter->setPen(pen);
    this->scene->drawLine(painter, this->line, true);
}


