#include "linedrawer.h"

QList<Pixel> LineDrawer::DDA(QPoint start, QPoint end, QColor color)
{
    QList<Pixel> points;
    QPointF diff = end - start;
    if (diff.isNull())
    {
        points << Pixel{ start, color };
    }
    else
    {
        qreal length;
        qreal dX, dY, x, y;

        length = std::max(qAbs(diff.x()),
                          qAbs(diff.y()));

        dX = diff.x() / length;
        dY = diff.y() / length;

        x = start.x();
        y = start.y();

        for (int i = 0; i <= length; i++)
        {
            points << Pixel{
                .point = QPoint(std::round(x),
                                std::round(y)),
                .color = color
            };
            x += dX;
            y += dY;
        }
    }
    return points;
}

QList<Pixel> LineDrawer::Bresenham(QPoint start, QPoint end, QColor color)
{
    QList<Pixel> points;
    QPointF diff = end - start;
    if (diff.isNull())
    {
        points << Pixel{ start, color };
    }
    else
    {
        qreal dX, dY, signX, signY, e, x, y, tan;
        bool swapped = false;

        signX = sign(diff.x());
        signY = sign(diff.y());

        dX = qAbs(diff.x());
        dY = qAbs(diff.y());

        if (dY > dX)
        {
            std::swap(dX, dY);
            swapped = true;
        }

        tan = dY / dX;
        e = tan - 0.5;
        x = start.x();
        y = start.y();

        while (x != end.x() || y != end.y())
        {
            points << Pixel{ QPoint(x, y), color };
            if (e >= 0)
            {
                (swapped)
                    ? x += signX
                    : y += signY;
                e -= 1;
            }
            if (e <= 0)
            {
                (swapped)
                    ? y += signY
                    : x += signX;
                e += tan;
            }
        }
    }
    return points;
}

QList<Pixel> LineDrawer::BresenhamInt(QPoint start, QPoint end, QColor color)
{
    QList<Pixel> points;
    QPoint diff = end - start;
    if (diff.isNull())
    {
        points << Pixel{ start, color };
    }
    else
    {
        int dX, dY, signX, signY, e, x, y;
        bool swapped = false;

        signX = sign(diff.x());
        signY = sign(diff.y());

        dX = qAbs(diff.x());
        dY = qAbs(diff.y());

        if (dY > dX)
        {
            std::swap(dX, dY);
            swapped = true;
        }

        e = 2 * dY - dX;
        x = start.x();
        y = start.y();

        while (x != end.x() || y != end.y())
        {
            points << Pixel{ QPoint(x, y), color };

            if (e >= 0)
            {
                (swapped)
                    ? x += signX
                    : y += signY;
                e -= 2 * dX;
            }
            if (e <= 0)
            {
                (swapped)
                    ? y += signY
                    : x += signX;
                e += 2 * dY;
            }
        }
    }
    return points;
}

QList<Pixel> LineDrawer::BresenhamSmooth(QPoint start,
                                      QPoint end,
                                      QColor foreground,
                                      QColor background)
{
    QList<Pixel> points;

    const int INTENSITY = 100;
    QList<QColor> gradient = intensityRange(foreground, background, INTENSITY);
    QPointF diff = end - start;
    qreal dX, dY, signX, signY, e, x, y, tan, threshold;
    bool swapped = false;

    signX = sign(diff.x());
    signY = sign(diff.y());

    dX = qAbs(diff.x());
    dY = qAbs(diff.y());

    if (dY > dX)
    {
        std::swap(dX, dY);
        swapped = true;
    }

    tan = dY / dX * INTENSITY;
    e = INTENSITY / 2;
    threshold = INTENSITY - tan;
    x = start.x();
    y = start.y();

    while (x != end.x() || y != end.y())
    {
        points << Pixel{ QPoint(x, y), gradient.at(std::round(e) - 1) };
        if (e < threshold)
        {
            (swapped)
                ? y += signY
                : x += signX;
            e += tan;
        }
        else
        {
            x += signX;
            y += signY;
            e -= threshold;
        }
    }

    return points;
}

QList<Pixel> LineDrawer::Wu(QPoint start, QPoint end, QColor foreground, QColor background)
{
    QList<Pixel> points;

    const int INTENSITY = 100;
    QList<QColor> gradient = intensityRange(foreground, background, INTENSITY);
    QPointF diff = end - start;
    if (diff.isNull())
    {
        points << Pixel{ start, gradient.last() };
    }

    qreal dX, dY, y, tan;
    bool steep;

    steep = qAbs(diff.y()) > qAbs(diff.x());

    if (steep)
    {
        qreal temp = start.x();
        start.setX(start.y());
        start.setY(temp);
        temp = end.x();
        end.setX(end.y());
        end.setY(temp);
    }
    if (start.x() > end.x())
    {
        std::swap(start, end);
    }

    dX = end.x() - start.x();
    dY = end.y() - start.y();

    if (dX == 0)
    {
        tan = 1;
    }
    else
    {
        tan = dY / dX;
    }

    qreal endX = std::round(start.x());
    qreal endY = start.y() + tan * (endX - start.x());
    y = endY + tan;
    qreal rangeStartX = endX;
    endX = int(end.x() + 0.5);
    qreal rangeEndX = endX;

    for (int x = rangeStartX; x < rangeEndX; x++)
    {
        if (steep)
        {
            int intensity = (INTENSITY - 1) * qAbs(1 - y + (int)y);
            Pixel pixel{ .point = QPoint((int)y, x + 1) };
            if (intensity < INTENSITY)
            {
                pixel.color = gradient.at(intensity);
            }
            else
            {
                pixel.color = foreground;
            }
            points << pixel;
            y += tan;
        }
        else
        {
            int intensity = std::round((INTENSITY - 1) * qAbs(1 - y + std::floor(y)));
            points << Pixel{
                QPoint(x + 1, (int)y),
                gradient.at(intensity)
            };
            points << Pixel{
                QPoint(x + 1, (int)y + 1),
                gradient.at(intensity)
            };
            y += tan;
        }
    }

    return points;
}

int sign(qreal value)
{
    return (value > 0) ? 1
           : (value < 0) ? -1 : 0;
}

QList<QColor> intensityRange(QColor foreground, QColor background, const int I)
{
    QList<QColor> gradient;
    int fR, fG, fB, bR, bG, bB;
    fR = foreground.red(); fG = foreground.green(); fB = foreground.blue();
    bR = background.red(); bG = background.green(); bB = foreground.blue();

    qreal stepR, stepG, stepB;
    stepR = getStep(fR, bR, I);
    stepG = getStep(fG, bG, I);
    stepB = getStep(fB, bB, I);

    for (int i = I - 1; i >= 0; i--)
    {
        QColor color;
        color.setRed(fR + stepR * i);
        color.setGreen(fG  + stepG * i);
        color.setBlue(fB  + stepB * i);
        gradient << color;
    }
    return gradient;
}

qreal getStep(int a1, int a2, const int I)
{
    return (a2 - a1) / I;
}
