#include "scenetriangle.h"
#include "coordinatescene.h"

SceneTriangle::SceneTriangle(MyTriangleF triangle_, CoordinateScene *scene_) : SceneObject(scene_)
{
    this->triangle = triangle_;
    this->setZValue(1);
}

QRectF SceneTriangle::boundingRect() const
{
    return this->triangle.boundingRect();
}

void SceneTriangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF boundingRect = this->boundingRect();
    if (!this->scene->getGraphicsWindow().intersects(boundingRect))
    {
        return;
    }
    painter->setTransform(this->scene->getTransform());
    QPen pen = QPen(Qt::cyan);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->setBrush(QColor(Qt::darkCyan));

    QPointF bisectIntersection = this->triangle.bisectIntersection();
    this->scene->drawMyTriangle(painter, this->triangle);


    pen.setWidth(1);
    pen.setColor(Qt::red);
    painter->setPen(pen);
    for (qsizetype i = 0; i < 3; i++)
    {
        this->scene->drawLine(painter, QLineF(this->triangle.getPoints().at(i), bisectIntersection));
    }
    this->scene->drawPoint(painter, bisectIntersection, QString("I"));
}
