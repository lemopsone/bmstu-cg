#ifndef MYRECTF_H
#define MYRECTF_H

#include "myshapef.h"

class MyRectF : public MyShapeF
{
public:
    MyRectF();
    MyRectF(QPointF points_[4]);
    MyRectF(QPointF p1, QPointF p2, QPointF p3, QPointF p4);

    bool isValid() override;
    qreal area() override;
    qreal perimeter() override;
};

#endif // MYRECTF_H
