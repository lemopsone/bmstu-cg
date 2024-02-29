#ifndef MYTRIANGLEF_H
#define MYTRIANGLEF_H

#include "myshapef.h"

class MyTriangleF : public MyShapeF
{
public:
    MyTriangleF(QPointF p1, QPointF p2, QPointF p3);
    QPointF bisectIntersection(void);
    QPointF bisectIntersection2(void);

    qreal innerCircleRadius(void);
    QRectF boundingRect(void) const;

    bool isValid(void) override;
    qreal area(void) override;
    qreal perimeter(void) override;
private:
    QPointF points[3];
};

#endif // MYTRIANGLEF_H
