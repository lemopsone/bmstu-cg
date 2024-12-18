#include "sceneline.h"

#include "coordinatescene.h"

SceneLine::SceneLine(QLineF line_, CoordinateScene *scene_)
    : SceneObject(scene_)
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

    painter->setTransform(this->scene->getTransform());
    QPen pen = QPen(Qt::darkBlue);
    pen.setWidth(2);
    painter->setPen(pen);
    this->scene->drawLine(painter, this->line);
}

void SceneLine::move(Move move)
{
    QPointF p1 = this->line.p1();
    QPointF p2 = this->line.p2();
    MyShapeF::movePoint(p1, move);
    MyShapeF::movePoint(p2, move);

    this->line.setPoints(p1, p2);
}

void SceneLine::rotate(Rotate rotate)
{
    QPointF p1 = this->line.p1();
    QPointF p2 = this->line.p2();
    MyShapeF::rotatePoint(p1, rotate);
    MyShapeF::rotatePoint(p2, rotate);

    this->line.setPoints(p1, p2);
}

void SceneLine::scale(Scale scale)
{
    QPointF p1 = this->line.p1();
    QPointF p2 = this->line.p2();
    MyShapeF::scalePoint(p1, scale);
    MyShapeF::scalePoint(p2, scale);

    this->line.setPoints(p1, p2);
}
