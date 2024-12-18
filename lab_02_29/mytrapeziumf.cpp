#include "mytrapeziumf.h"

MyTrapeziumF::MyTrapeziumF() {}

MyTrapeziumF::MyTrapeziumF(QPointF points_[4]) {
    for (auto point : points) {
        this->points.push_back(point);
    }
}

MyTrapeziumF::MyTrapeziumF(QPointF a1, QPointF a2, QPointF b1, QPointF b2) {
    this->points.push_back(a1);
    this->points.push_back(a2);
    this->points.push_back(b1);
    this->points.push_back(b2);
}

bool MyTrapeziumF::isValid() {
    QVector2D vecA = QVector2D(this->points[1] - this->points[0]);
    QVector2D vecB = QVector2D(this->points[3] - this->points[2]);

    qreal crossProduct = vecA.x() * vecB.y() - vecA.y() * vecB.x();

    return !qFuzzyIsNull(crossProduct);
}

qreal MyTrapeziumF::area() {
    /* not implemented */
    Q_UNIMPLEMENTED();
    return 0;
}

qreal MyTrapeziumF::perimeter()
{
    qreal length = 0;
    length += QLineF(this->points[0], this->points[1]).length();
    length += QLineF(this->points[1], this->points[2]).length();
    length += QLineF(this->points[2], this->points[3]).length();
    length += QLineF(this->points[3], this->points[0]).length();

    return length;
}
