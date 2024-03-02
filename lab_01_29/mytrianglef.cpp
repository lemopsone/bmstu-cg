#include "mytrianglef.h"
#include <QDebug>

MyTriangleF::MyTriangleF(QPointF p1, QPointF p2, QPointF p3)
{
    this->points[0] = p1;
    this->points[1] = p2;
    this->points[2] = p3;
}

bool MyTriangleF::isValid()
{
    QVector2D vecA = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[0]);
    QVector2D vecC = QVector2D(this->points[1] - this->points[0]);
    qreal a = vecA.length(), b = vecB.length(), c = vecC.length();

    // Одна из длин сторон равна нулю
    if (a * b * c == 0.0)
    {
        return false;
    }

    /*
     * Неравенство треугольника:
     * AB <= BC + AC для любых A, B, C
     * Знак равенства (AB = BC + AC) указывает на
     * то, что точка B лежит строго между A и C,
     * т. е. треугольник - вырожденный
     */
    if (a < b + c &&
        b < a + c &&
        c < a + b)
    {
        return true;
    }

    return false;
}

QPointF MyTriangleF::bisectIntersection()
{
    /*
     * A - points[0]
     * B - points[1]
     * C - points[2]
     * ~a = ~BC
     * ~b = ~AC
     * ~c = ~AB
     */
    QVector2D vecA = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[0]);
    QVector2D vecC = QVector2D(this->points[1] - this->points[0]);
    qreal a = vecA.length(), b = vecB.length(), c = vecC.length();

    QVector2D vecAK = vecC + vecA * c / (b + c);
    QVector2D vecAI = vecAK * (b + c) / (a + b + c);
    qDebug() << vecAI.toPointF() + this->points[0];
    return vecAI.toPointF() + this->points[0];
}

QPointF MyTriangleF::bisectIntersection2()
{
    /*
     * A - points[0]
     * B - points[1]
     * C - points[2]
     * ~a = ~BC
     * ~b = ~AC
     * ~c = ~AB
     */
    QVector2D vecA = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[0]);
    QVector2D vecC = QVector2D(this->points[1] - this->points[0]);
    qreal a = vecA.length(), b = vecB.length(), c = vecC.length();

    QVector2D vecBI = (c * vecA - a * vecC) / (a + b + c);
    return vecBI.toPointF() + this->points[1];
}

qreal MyTriangleF::area()
{
    QVector2D vecA = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[0]);
    QVector2D vecC = QVector2D(this->points[1] - this->points[0]);
    qreal a = vecA.length(), b = vecB.length(), c = vecC.length();
    qreal p = this->perimeter() / 2;
    return std::sqrt(p * (p - a) * (p - b) * (p - c));
}

qreal MyTriangleF::perimeter()
{
    QVector2D vecA = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[0]);
    QVector2D vecC = QVector2D(this->points[1] - this->points[0]);
    qreal a = vecA.length(), b = vecB.length(), c = vecC.length();

    return a + b + c;
}

qreal MyTriangleF::innerCircleRadius()
{
    QVector2D vecA = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[0]);
    QVector2D vecC = QVector2D(this->points[1] - this->points[0]);
    qreal a = vecA.length(), b = vecB.length(), c = vecC.length();
    qreal perim = a + b + c;

    return 2.0 * this->area() / perim;
}
