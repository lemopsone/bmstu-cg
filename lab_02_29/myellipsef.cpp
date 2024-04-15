#include "myellipsef.h"

MyEllipseF::MyEllipseF() {}

MyEllipseF::MyEllipseF(QPointF center, QPointF a, QPointF b)
{
    this->points.push_back(center);
    this->points.push_back(a);
    this->points.push_back(b);
}

QPointF MyEllipseF::center() const
{
    return this->points.at(0);
}

QPointF MyEllipseF::a() const
{
    return this->points.at(1);
}

QPointF MyEllipseF::b() const
{
    return this->points.at(2);
}

bool MyEllipseF::isValid()
{
    return true;
}

qreal MyEllipseF::area()
{
    qreal a = QLineF(this->a(), this->center()).length();
    qreal b = QLineF(this->b(), this->center()).length();

    return M_PI * a * b;
}

qreal MyEllipseF::perimeter()
{
    return 0;
}
