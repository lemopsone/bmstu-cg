#ifndef _SHAPEUTILS_H_
#define _SHAPEUTILS_H_

#include <QColor>
#include <QList>
#include <QPoint>

#define MULTIPLIER 100

enum DrawingAlgorithm { CANONICAL, PARAMETRIC, BRESENHAM, MIDDLE_POINT };

struct Pixel {
  QPoint point;
  QColor color = Qt::black;
};

struct CircleData {
  QPoint center = QPoint(0, 0);
  qsizetype radius;
  QColor color = Qt::black;
  DrawingAlgorithm algorithm;
};

struct EllipseData {
  QPoint center = QPoint(0, 0);
  qsizetype a;
  qsizetype b;
  QColor color = Qt::black;
  DrawingAlgorithm algorithm;
};

typedef QList<Pixel> (*drawingFunction)(const CircleData &, QColor);

QList<Pixel> getSymmetricPixels(const QPoint &point, const QPoint &center,
                                const QColor &color);

#endif // _SHAPEUTILS_H_
