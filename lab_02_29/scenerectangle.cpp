#include "scenerectangle.h"

#include "coordinatescene.h"

SceneRectangle::SceneRectangle(MyRectF rect_, CoordinateScene *scene_)
    : SceneObject(scene_)
{
    this->rect = rect_;
}

QRectF SceneRectangle::boundingRect() const
{
    return this->rect.boundingRect();
}

void SceneRectangle::paint(QPainter *painter,
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

    this->scene->drawMyRect(painter, rect);
    // this->scene->drawPoint(painter, this->rect.center());
    // QList<QPointF> points = this->rect.getPoints();
    // for (qsizetype i = 0; i < 4; i++) {
    //     this->scene->drawPoint(painter, points[i], QString() + (char) ('A' + i));
    // }
}

void SceneRectangle::move(Move move)
{
    this->rect.move(move);
}

void SceneRectangle::rotate(Rotate rotate)
{
    this->rect.rotate(rotate);
}

void SceneRectangle::scale(Scale scale)
{
    this->rect.scale(scale);
}
