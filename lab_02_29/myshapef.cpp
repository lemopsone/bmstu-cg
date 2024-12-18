#include "myshapef.h"

MyShapeF::MyShapeF() {}

QRectF MyShapeF::boundingRect() const
{
    return QRectF(this->left(),
                  this->top(),
                  this->right() - this->left(),
                  this->bottom() - this->top());
}

qreal MyShapeF::top() const
{
    qreal min = INFINITY;
    for (auto point : this->points) {
        if (point.y() < min) {
            min = point.y();
        }
    }
    return min;
}

qreal MyShapeF::bottom() const
{
    qreal max = -INFINITY;
    for (auto point : this->points) {
        if (point.y() > max) {
            max = point.y();
        }
    }
    return max;
}

qreal MyShapeF::left() const
{
    qreal min = INFINITY;
    for (auto point : this->points) {
        if (point.x() < min) {
            min = point.x();
        }
    }
    return min;
}

qreal MyShapeF::right() const
{
    qreal max = -INFINITY;
    for (auto point : this->points) {
        if (point.x() > max) {
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

void MyShapeF::move(Move move)
{
    for (QPointF& point : this->points) {
        point += QPointF(move.x, move.y);
    }
}

void MyShapeF::rotate(Rotate rotate)
{
    for (QPointF &point : this->points) {
        this->rotatePoint(point, rotate);
    }
}

void MyShapeF::scale(Scale scale)
{
    for (QPointF &point : this->points) {
        this->scalePoint(point, scale);
    }
}

void MyShapeF::movePoint(QPointF &point, Move move)
{
    point.setX(point.x() + move.x);
    point.setY(point.y() + move.y);
}

void MyShapeF::rotatePoint(QPointF &point, Rotate rotate)
{
    qreal x, y, cosine, sine;
    sine = std::sin(rotate.angle);
    cosine = std::cos(rotate.angle);
    x = rotate.center->x() +
        (point.x() - rotate.center->x()) * cosine +
        (point.y() - rotate.center->y()) * sine;
    y = rotate.center->y() +
        (point.y() - rotate.center->y()) * cosine -
        (point.x() - rotate.center->x()) * sine;
    point.setX(x);
    point.setY(y);
}

void MyShapeF::scalePoint(QPointF &point, Scale scale)
{
    qreal x, y;
    x = point.x() * scale.kx +
        (1 - scale.kx) * scale.center->x();
    y = point.y() * scale.ky +
        (1 - scale.ky) * scale.center->y();
    point.setX(x);
    point.setY(y);
}
