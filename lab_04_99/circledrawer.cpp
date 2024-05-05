#include "circledrawer.h"

QList<Pixel> CircleDrawer::Canonical(const CircleData &data) {
  QList<Pixel> pixels;
  double x = data.center.x();
  double step = 1.0 / data.radius;
  size_t steps = std::round(data.radius / step);

  QPoint prevPoint = data.center;
  for (size_t i = 0; i <= steps; i++) {
    if (x > data.center.x() + data.radius) {
      continue;
    }
    int y = data.center.y() +
            std::sqrt(data.radius * data.radius -
                      (x - data.center.x()) * (x - data.center.x()));
    if (prevPoint != QPoint(x, y)) {
      pixels += getSymmetricPixels(QPoint(x, y), data.center, data.color);
      prevPoint = QPoint(x, y);
    }
    x += step;
  }
  return pixels;
}

QList<Pixel> CircleDrawer::Bresenham(const CircleData &data) {
  QList<Pixel> pixels;

  int x = 0;
  int y = data.radius;
  pixels += getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                               data.center, data.color);

  int delta = 2 * (1 - data.radius);
  int d1;

  while (y >= 0) {
    if (delta < 0) {
      d1 = 2 * delta + 2 * y - 1;
      if (d1 <= 0) {
        x++;
        delta += 2 * x + 1;
      } else {
        x++;
        y--;
        delta += 2 * (x - y + 1);
      }
    } else if (delta > 0) {
      d1 = 2 * delta - 2 * x - 1;
      if (d1 <= 0) {
        x++;
        y--;
        delta += 2 * (x - y + 1);
      } else {
        y--;
        delta += -2 * y + 1;
      }
    } else {
      x++;
      y--;
      delta += 2 * (x - y + 1);
    }
    pixels +=
        getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                           data.center, data.color);
  }

  return pixels;
}

QList<Pixel> CircleDrawer::Parametric(const CircleData &data) {
  QList<Pixel> pixels;
  size_t steps = round(MULTIPLIER * data.radius);
  double step = M_PI / 4 / steps;
  double angle = 0;
  int x, y;
  QPoint prevPoint = data.center;
  for (size_t i = 0; i < steps; i++) {
    x = data.center.x() + std::round(data.radius * std::cos(angle));
    y = data.center.y() + std::round(data.radius * std::sin(angle));
    angle += step;

    if (prevPoint != QPoint(x, y)) {
      pixels += getSymmetricPixels(QPoint(x, y), data.center, data.color);
      pixels += getSymmetricPixels(QPoint(y, x), data.center, data.color);
      prevPoint = QPoint(x, y);
    }
  }
  return pixels;
}

QList<Pixel> CircleDrawer::MiddlePoint(const CircleData &data) {
  QList<Pixel> pixels;
  int x = data.radius;
  int y = 0;
  int delta = 1 - data.radius;

  pixels += getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                               data.center, data.color);

  while (x >= y) {
    if (delta >= 0) {
      x--;
      y++;
      delta += 2 * y + 1 - 2 * x;
    } else {
      y++;
      delta += 2 * y + 1;
    }

    pixels +=
        getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                           data.center, data.color);
  }

  x = 0;
  y = data.radius;
  delta = 1 - data.radius;

  while (y >= x) {
    if (delta >= 0) {
      x++;
      y--;
      delta += 2 * x + 1 - 2 * y;
    } else {
      x++;
      delta += 2 * x + 1;
    }

    pixels +=
        getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                           data.center, data.color);
  }

  return pixels;
}
