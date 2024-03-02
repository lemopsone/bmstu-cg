#ifndef MYRECTF_H
#define MYRECTF_H

#include "myshapef.h"

class MyRectF : public MyShapeF
{
public:
    MyRectF();
    MyRectF(QPointF p1, QPointF p2, QPointF p3, QPointF p4);

    bool isValid() override;
    qreal area() override;
    qreal perimeter() override;

private:
    QList<QPointF> points;
};

#endif // MYRECTF_H
