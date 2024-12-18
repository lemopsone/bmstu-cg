#ifndef POINTRECORD_H
#define POINTRECORD_H

#include <QPoint>
#include <QString>

class PointRecord
{
public:
    PointRecord();
    PointRecord(QString text);
    PointRecord(QPoint point);

    QPoint getPoint(void) const;
    void setPoint(const QPoint point);

    int getId(void) const;
    void setId(const int id);

    QString getText(void) const;
    void setText(const QString text);

    qreal getX(void) const;
    void setX(const qreal x);

    qreal getY(void) const;
    void setY(const qreal y);

    QString serializePoint(QPoint point);

private:
    QPoint point;
    int id;
    QString text;
    QString icon;
};

#endif // POINTRECORD_H
