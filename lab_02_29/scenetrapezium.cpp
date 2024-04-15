#include "scenetrapezium.h"

#include "coordinatescene.h"

SceneTrapezium::SceneTrapezium(MyTrapeziumF trapezium_, CoordinateScene *scene_)
    : SceneObject(scene_)
{
    this->trapezium = trapezium_;
}

QRectF SceneTrapezium::boundingRect() const
{
    return this->trapezium.boundingRect();
}

void SceneTrapezium::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setTransform(this->scene->getTransform());
    QPen pen = QPen(QColor(QString("#FC6600")));
    pen.setWidth(3);
    painter->setPen(pen);
    painter->setBrush(QColor(QString("#FC6600")));
    this->scene->drawTrapezium(painter, this->trapezium);
}

void SceneTrapezium::move(Move move)
{
    this->trapezium.move(move);
}

void SceneTrapezium::rotate(Rotate rotate)
{
    this->trapezium.rotate(rotate);
}

void SceneTrapezium::scale(Scale scale)
{
    this->trapezium.scale(scale);
}
