#ifndef FILL_H
#define FILL_H

#include "coordinatescene.h"
#include <QColor>
#include <QElapsedTimer>
#include <QList>
#include <QPoint>
#include <QTime>
#include <stack>

struct FillColors {
  QColor foreground = Qt::blue;
  QColor edge = Qt::black;
};

void traverse_line(QImage *image, FillColors colors, std::stack<QPoint> *stack,
                   int left, int right, int y);
long fill(CoordinateScene *scene, FillColors colors, QPoint seed, int delay);

#endif // FILL_H
