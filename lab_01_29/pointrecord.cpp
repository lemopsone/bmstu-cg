#include "pointrecord.h"

PointRecord::PointRecord(QPointF point)
{
    this->setPoint(point);
}

QPointF PointRecord::getPoint(void) const
{
    return this->point;
}

void PointRecord::setPoint(const QPointF point)
{
    this->point = point;
    this->setText(this->serializePoint(point));
}

int PointRecord::getId(void) const
{
    return this->id;
}

void PointRecord::setId(const int id)
{
    this->id = id;
}

QString PointRecord::getText(void) const
{
    return this->text;
}

void PointRecord::setText(const QString text)
{
    this->text = text;
}

qreal PointRecord::getX(void) const
{
    return this->point.x();
}

void PointRecord::setX(const qreal x)
{
    this->point.setX(x);
}

qreal PointRecord::getY(void) const
{
    return this->point.y();
}

void PointRecord::setY(const qreal y)
{
    this->point.setY(y);
}

QString PointRecord::serializePoint(QPointF point)
{
    return QString("(%1, %2)").arg(point.x()).arg(point.y());
}
