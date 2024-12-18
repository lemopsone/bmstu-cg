#ifndef POINTRECORD_H
#define POINTRECORD_H

#include <QPointF>
#include <QString>

class PointRecord
{
public:
    PointRecord();
    PointRecord(QPointF point);

    QPointF getPoint(void) const;
    void setPoint(const QPointF point);

    int getId(void) const;
    void setId(const int id);

    QString getText(void) const;
    void setText(const QString text);

    qreal getX(void) const;
    void setX(const qreal x);

    qreal getY(void) const;
    void setY(const qreal y);

    QString serializePoint(QPointF point);

private:
    QPointF point;
    int id;
    QString text;
    QString icon;
};

#endif // POINTRECORD_H
