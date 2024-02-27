#ifndef MYRECTF_H
#define MYRECTF_H

#include <QObject>
#include <QPointF>
#include <QRectF>

class MyRectF
{
public:
    MyRectF(QPointF p1, QPointF p2, QPointF p3, QPointF p4);

    bool isValid(void);

    QRectF boundingRect();
    qreal top();
    qreal bottom();
    qreal left();
    qreal right();
    QPointF center();

private:
    QPointF points[4];
};

#endif // MYRECTF_H
