#ifndef MYTRIANGLEF_H
#define MYTRIANGLEF_H

#include <QObject>
#include <QPointF>
#include <QVector2D>
#include <QDebug>

class MyTriangleF
{
public:
    MyTriangleF(QPointF p1, QPointF p2, QPointF p3);
    QPointF bisectIntersection(void);
    QPointF bisectIntersection2(void);
    qreal area(void);
    qreal innerCircleRadius(void);

private:
    QPointF points[3];
};

#endif // MYTRIANGLEF_H
