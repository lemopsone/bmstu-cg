#ifndef MYSHAPEF_H
#define MYSHAPEF_H

#include <QList>
#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QVector2D>

class MyShapeF
{
public:
    MyShapeF();

    QRectF boundingRect() const;
    qreal top() const;
    qreal bottom() const;
    qreal left() const;
    qreal right() const;
    QPointF center() const;
    QList<QPointF> getPoints();

    virtual bool isValid() = 0;
    virtual qreal area() = 0;
    virtual qreal perimeter() = 0;

protected:
    QList<QPointF> points;
};

#endif // MYSHAPEF_H
