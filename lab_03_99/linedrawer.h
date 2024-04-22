#ifndef LINEDRAWER_H
#define LINEDRAWER_H

#include <QPolygonF>
#include <QLineF>
#include <QColor>
#include <QList>

struct Pixel
{
    QPoint point;
    QColor color;
};

enum LineType {
    DDA,
    BRESENHAM_FLOAT,
    BRESENHAM_INT,
    BRESENHAM_SMOOTH,
    WU
};

class LineDrawer
{
public:
    static  QList<Pixel> DDA(QPoint start, QPoint end, QColor color);
    static QList<Pixel> Bresenham(QPoint start, QPoint end, QColor color);
    static QList<Pixel> BresenhamInt(QPoint start, QPoint end, QColor color);
    static QList<Pixel> BresenhamSmooth(QPoint start, QPoint end,
                                     QColor foreground = Qt::black, QColor background = Qt::white);
    static QList<Pixel> Wu(QPoint start, QPoint end,
                            QColor foreground = Qt::black, QColor background = Qt::white);
};

int sign(qreal value);

QList<QColor> intensityRange(QColor foreground, QColor background, const int I = 100);

qreal getStep(int a1, int a2, const int I);

#endif // LINEDRAWER_H
