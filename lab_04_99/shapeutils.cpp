#include "shapeutils.h"

QList<Pixel> getSymmetricPixels(const QPoint &point, const QPoint &center,
                                const QColor &color) {
  QList<Pixel> pixels(4);
  QPoint difference = point - center;
  pixels[0].point = center + QPoint(difference.x(), difference.y());
  pixels[1].point = center + QPoint(-difference.x(), difference.y());
  pixels[2].point = center + QPoint(difference.x(), -difference.y());
  pixels[3].point = center + QPoint(-difference.x(), -difference.y());
  for (auto &pixel : pixels) {
    pixel.color = color;
  }
  return pixels;
}
