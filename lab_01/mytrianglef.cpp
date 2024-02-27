#include "mytrianglef.h"

MyTriangleF::MyTriangleF(QPointF p1, QPointF p2, QPointF p3)
{
    this->points[0] = p1;
    this->points[1] = p2;
    this->points[2] = p3;
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
    qDebug() << vecBI.toPointF() + this->points[1];
    return vecBI.toPointF() + this->points[1];
}

qreal MyTriangleF::area()
{
    QVector2D vecA = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[0]);
    QVector2D vecC = QVector2D(this->points[1] - this->points[0]);
    qreal a = vecA.length(), b = vecB.length(), c = vecC.length();
    qreal p = (a + b + c) / 2.0;
    return std::sqrt(p * (p - a) * (p - b) * (p - c));
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
