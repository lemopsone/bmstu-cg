#ifndef MYTRIANGLEF_H
#define MYTRIANGLEF_H

#include "myshapef.h"

class MyTriangleF : public MyShapeF
{
public:
    MyTriangleF();
    MyTriangleF(QPointF p1, QPointF p2, QPointF p3);
    QPointF bisectIntersection(void);
    QPointF bisectIntersection2(void);

    qreal innerCircleRadius(void);

    bool isValid(void) override;
    qreal area(void) override;
    qreal perimeter(void) override;
private:
    QList<QPointF> points;
};

#endif // MYTRIANGLEF_H
