#ifndef MYELLIPSEF_H
#define MYELLIPSEF_H

#include <QLineF>
#include "myshapef.h"

class MyEllipseF : public MyShapeF
{
public:
    MyEllipseF();
    MyEllipseF(QPointF center, QPointF a, QPointF b);

    QPointF center() const override;
    QPointF a() const;
    QPointF b() const;

    bool isValid() override;
    qreal area() override;
    qreal perimeter() override;
};

#endif // MYELLIPSEF_H
