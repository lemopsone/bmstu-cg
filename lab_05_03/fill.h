#ifndef FILL_H
#define FILL_H

#include <QList>
#include <QPoint>
#include <QColor>

struct Pixel {
    QPoint point;
    QColor color = Qt::black;
};

struct Edge {
    QPoint start;
    QPoint end;
    bool isHorizontal() const;
    bool isVertical() const;
};

typedef QList<Pixel> PixelRow;

class Fill {
public:
    static QPoint topLeft(QList<QPoint> points);
    static QPoint bottomRight(QList<QPoint> points);
    static QList<PixelRow> create(QList<QPoint> points, QColor color = Qt::black);
    static int septum(QList<QPoint> points);
    static QList<Edge> createEdges(QList<QPoint> points);
};

#endif // FILL_H
