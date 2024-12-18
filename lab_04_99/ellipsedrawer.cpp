#include "ellipsedrawer.h"

QList<Pixel> EllipseDrawer::Canonical(const EllipseData &data) {
  QList<Pixel> pixels;

  double step = 1.0 / data.a;
  size_t steps = std::round(data.a / step);

  QPoint prevPoint = data.center;

  for (size_t i = 0; i < steps; i++) {
    double x = data.center.x() + i * step;
    if (x * x > data.a * data.a) {
      break;
    }
    double y =
        data.center.y() + data.b * std::sqrt(1 - x * x / (data.a * data.a));
    if (QPoint(x, y) != prevPoint) {
      pixels += getSymmetricPixels(QPoint(x, y), data.center, data.color);
      prevPoint = QPoint(x, y);
    }
  }

  return pixels;
}

QList<Pixel> EllipseDrawer::Parametric(const EllipseData &data) {
  QList<Pixel> pixels;
  size_t steps = round(MULTIPLIER * std::sqrt(data.a * data.b));
  double step = M_PI / 2 / steps;

  for (size_t i = 0; i <= steps; i++) {
    int x = data.center.x() + data.a * cos(i * step);
    int y = data.center.y() + data.b * sin(i * step);
    pixels += getSymmetricPixels(QPoint(x, y), data.center, data.color);
  }
  return pixels;
}

QList<Pixel> EllipseDrawer::Bresenham(const EllipseData &data) {
  QList<Pixel> pixels;
  int x = 0;
  int y = data.b;

  pixels += getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                               data.center, data.color);

  int squareA = data.a * data.a;
  int squareB = data.b * data.b;
  int delta = squareB - squareA * (2 * data.b + 1);

  while (y >= 0) {
    if (delta < 0) {
      int d1 = 2 * delta + 2 * squareA * (2 * y + 2);
      x++;
      if (d1 < 0) {
        delta += squareB * (2 * x + 1);
      } else {
        y--;
        delta += squareB * (2 * x + 1) + squareA * (1 - 2 * y);
      }
    } else if (delta > 0) {
      int d2 = 2 * delta + squareB * (2 - 2 * x);
      y--;
      if (d2 > 0) {
        delta += squareA * (1 - 2 * y);
      } else {
        x++;
        delta += squareB * (2 * x + 1) + squareA * (1 - 2 * y);
      }
    } else {
      x++;
      y--;
      delta += squareB * (2 * x + 1) + squareA * (1 - 2 * y);
    }

    pixels +=
        getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                           data.center, data.color);
  }

  return pixels;
}

QList<Pixel> EllipseDrawer::MiddlePoint(const EllipseData &data) {
  QList<Pixel> pixels;
  int x = 0;
  int y = data.b;
  pixels += getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                               data.center, data.color);
  int squareA = data.a * data.a;
  int squareB = data.b * data.b;

  int d1 = squareB - std::round(squareA * ((double)data.b - 0.25));
  while (2 * squareB * x < 2 * squareA * y) {
    if (d1 < 0) {
      x++;
      d1 += squareB * (2 * x + 1);
    } else {
      x++;
      y--;
      d1 += squareB * (2 * x + 1) - 2 * squareA * y;
    }
    pixels +=
        getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                           data.center, data.color);
  }

  int d2 = squareA - std::round(squareB * ((double)data.a - 0.25));
  while (y >= 0) {
    if (d2 < 0) {
      y--;
      d2 += squareA * (2 * y + 1);
    } else {
      x++;
      y--;
      d2 += squareA * (2 * y + 1) - 2 * squareB * x;
    }
    pixels +=
        getSymmetricPixels(QPoint(x + data.center.x(), y + data.center.y()),
                           data.center, data.color);
  }

  return pixels;
}
