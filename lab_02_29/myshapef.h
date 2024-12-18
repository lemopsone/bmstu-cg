#ifndef MYSHAPEF_H
#define MYSHAPEF_H

#include <QList>
#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QVector2D>
#include <QtMath>

#include "actions.h"

class MyShapeF
{
public:
    MyShapeF();

    virtual QRectF boundingRect() const;
    virtual qreal top() const;
    virtual qreal bottom() const;
    virtual qreal left() const;
    virtual qreal right() const;
    virtual QPointF center() const;

    virtual QList<QPointF> getPoints();

    virtual bool isValid() = 0;
    virtual qreal area() = 0;
    virtual qreal perimeter() = 0;

    virtual void move(Move move);
    virtual void rotate(Rotate rotate);
    virtual void scale(Scale scale);

    static void movePoint(QPointF &point, Move move);
    static void rotatePoint(QPointF &point, Rotate rotate);
    static void scalePoint(QPointF &point, Scale scale);
protected:
    QList<QPointF> points;
};

#endif // MYSHAPEF_H
