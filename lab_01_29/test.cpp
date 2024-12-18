#include "test.h"

Test::Test() {}

Test::Test(QString name, QList<QPointF> points, MyRectF rect)
{
    this->name_ = name;
    this->points_ = points;
    this->rect_ = rect;
}

QString Test::name() const
{
    return this->name_;
}

QList<QPointF> Test::points() const
{
    return this->points_;
}

MyRectF Test::rect() const
{
    return this->rect_;
}

bool Test::isEmpty()
{
    if (!this->name_.isEmpty())
        return false;
    if (!this->points_.isEmpty())
        return false;

    return true;
}

QDebug operator<<(QDebug stream, const Test &test)
{
    stream << "Текущий тест: " << test.name() << Qt::endl;
    stream << "\tПрямоугольник:\n";
    for (auto point : test.rect().getPoints()) {
        stream << "\t\t" << "(" << point.x() << "; " << point.y() << ")" << Qt::endl;
    }
    stream << Qt::endl << "\tТочки на плоскости:" << Qt::endl;
    for (auto point : test.points()) {
        stream << "\t\t" << "(" << point.x() << "; " << point.y() << ")" << Qt::endl;
    }

    return stream;
}
