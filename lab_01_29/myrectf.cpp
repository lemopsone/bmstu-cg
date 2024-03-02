#include "myrectf.h"

MyRectF::MyRectF()
{
    for (short i = 0; i < 4; i++)
    {
        this->points.push_back(QPointF());
    }
}

MyRectF::MyRectF(QPointF p1, QPointF p2, QPointF p3, QPointF p4)
{
    this->points.push_back(p1);
    this->points.push_back(p2);
    this->points.push_back(p3);
    this->points.push_back(p4);
}

bool MyRectF::isValid()
{
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = i + 1; j < 4; j++)
        {
            if (points[i] == points[j])
            {
                return false;
            }
        }
    }

    qreal tolerance = 0.001;

    QVector2D vecA = QVector2D(this->points[1] - this->points[0]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecC = QVector2D(this->points[3] - this->points[2]);
    QVector2D vecD = QVector2D(this->points[0] - this->points[3]);

    qreal dotAB = QVector2D::dotProduct(vecA, vecB);
    qreal dotBC = QVector2D::dotProduct(vecB, vecC);
    qreal dotCD = QVector2D::dotProduct(vecC, vecD);
    qreal dotAD = QVector2D::dotProduct(vecD, vecA);

    if (qAbs(dotAB) < tolerance &&
        qAbs(dotBC) < tolerance &&
        qAbs(dotCD) < tolerance &&
        qAbs(dotAD) < tolerance)
    {
        return true;
    }
    return false;
}

qreal MyRectF::area()
{
    QVector2D vecA = QVector2D(this->points[1] - this->points[0]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[1]);

    return vecA.length() * vecB.length();
}

qreal MyRectF::perimeter()
{
    QVector2D vecA = QVector2D(this->points[1] - this->points[0]);
    QVector2D vecB = QVector2D(this->points[2] - this->points[1]);
    QVector2D vecC = QVector2D(this->points[3] - this->points[2]);
    QVector2D vecD = QVector2D(this->points[0] - this->points[3]);
    qreal a = vecA.length(), b = vecB.length(), c = vecC.length(), d = vecD.length();

    return a + b + c + d;
}
