#include "myshapef.h"

MyShapeF::MyShapeF() {}

QRectF MyShapeF::boundingRect() const
{
    return QRectF(
        this->left(),
        this->top(),
        this->right() - this->left(),
        this->bottom() - this->top()
        );
}

qreal MyShapeF::top() const
{
    qreal min = INFINITY;
    for (auto point : this->points)
    {
        if (point.y() < min)
        {
            min = point.y();
        }
    }
    return min;
}

qreal MyShapeF::bottom() const
{
    qreal max = -INFINITY;
    for (auto point : this->points)
    {
        if (point.y() > max)
        {
            max = point.y();
        }
    }
    return max;
}

qreal MyShapeF::left() const
{
    qreal min = INFINITY;
    for (auto point : this->points)
    {
        if (point.y() < min)
        {
            min = point.x();
        }
    }
    return min;
}

qreal MyShapeF::right() const
{
    qreal max = -INFINITY;
    for (auto point : this->points)
    {
        if (point.x() > max)
        {
            max = point.x();
        }
    }
    return max;
}

QPointF MyShapeF::center() const
{
    return this->boundingRect().center();
}

QList<QPointF> MyShapeF::getPoints()
{
    return this->points;
}
