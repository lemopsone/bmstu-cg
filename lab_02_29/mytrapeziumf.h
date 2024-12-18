#ifndef MYTRAPEZIUMF_H
#define MYTRAPEZIUMF_H

#include <QLineF>
#include "myshapef.h"

class MyTrapeziumF : public MyShapeF
{
public:
    MyTrapeziumF();
    MyTrapeziumF(QPointF points_[4]);
    MyTrapeziumF(QPointF a1, QPointF a2, QPointF b1, QPointF b2);
    bool isValid() override;
    qreal area() override;
    qreal perimeter() override;
};

#endif // MYTRAPEZIUMF_H
