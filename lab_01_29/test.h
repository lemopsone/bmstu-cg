#ifndef TEST_H
#define TEST_H

#include <QString>
#include <QList>
#include <QDebug>
#include "myrectf.h"

class Test
{
public:
    Test();
    Test(QString name, QList<QPointF> points, MyRectF rect);
    QString name() const;
    QList<QPointF> points() const;
    MyRectF rect() const;
    bool isEmpty();
private:
    QString name_;
    QList<QPointF> points_;
    MyRectF rect_;
};
QDebug operator<<(QDebug stream, const Test &test);

#endif // TEST_H
