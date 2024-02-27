#include "myrectf.h"

MyRectF::MyRectF(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    this->points[0] = p1;
    this->points[1] = p2;
    this->points[2] = p3;
    this->points[3] = p4;
}

bool MyRectF::isValid()
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = i + 1; j < 4; j++)
        {
            if (points[i] == points[j])
            {
                return false;
            }
        }
    }

    qreal tolerance = 0.001;

    QPointF v1 = this->points[2] - this->points[1];
    QPointF v2 = this->points[3] - this->points[2];
    QPointF v3 = this->points[4] - this->points[3];
    QPointF v4 = this->points[1] - this->points[4];

    qreal dotProduct1 = QPointF::dotProduct(v1, v2);
    qreal dotProduct2 = QPointF::dotProduct(v2, v3);
    qreal dotProduct3 = QPointF::dotProduct(v3, v4);
    qreal dotProduct4 = QPointF::dotProduct(v4, v1);

    if (qAbs(dotProduct1) < tolerance &&
        qAbs(dotProduct2) < tolerance &&
        qAbs(dotProduct3) < tolerance &&
        qAbs(dotProduct4) < tolerance)
    {
        return true;
    }
    return false;
}

QRectF MyRectF::boundingRect()
{
    return QRectF(
        this->left(),
        this->top(),
        this->right() - this->left(),
        this->bottom() - this->top()
    );
}

qreal MyRectF::top()
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

qreal MyRectF::bottom()
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

qreal MyRectF::left()
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

qreal MyRectF::right()
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

QPointF MyRectF::center()
{
    return this->boundingRect().center();
}
