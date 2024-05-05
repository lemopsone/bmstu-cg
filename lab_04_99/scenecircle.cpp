#include "scenecircle.h"

#include "coordinatescene.h"

SceneCircle::SceneCircle(CircleData data, CoordinateScene *scene_)
    : SceneObject(scene_) {
  this->data_ = data;
}

QRectF SceneCircle::boundingRect() const {
  return QRectF(data_.center.x() - data_.radius,
                data_.center.y() - data_.radius, 2 * data_.radius,
                2 * data_.radius);
}

void SceneCircle::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option,
                        QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  QList<Pixel> pixels;
  switch (data_.algorithm) {
  case MIDDLE_POINT:
    pixels = CircleDrawer::MiddlePoint(data_);
    break;
  case PARAMETRIC:
    pixels = CircleDrawer::Parametric(data_);
    break;
  case BRESENHAM:
    pixels = CircleDrawer::Bresenham(data_);
    break;
  case CANONICAL:
    pixels = CircleDrawer::Canonical(data_);
    break;
  }
  scene->drawPixels(painter, pixels);
}
