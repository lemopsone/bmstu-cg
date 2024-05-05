#include "sceneellipse.h"

#include "coordinatescene.h"

SceneEllipse::SceneEllipse(EllipseData data, CoordinateScene *scene_)
    : SceneObject(scene_) {
  this->data_ = data;
}

QRectF SceneEllipse::boundingRect() const {
  return QRectF(data_.center.x() - data_.a, data_.center.y() - data_.b,
                2 * data_.a, 2 * data_.b);
}

void SceneEllipse::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  QList<Pixel> pixels;
  switch (data_.algorithm) {
  case MIDDLE_POINT:
    pixels = EllipseDrawer::MiddlePoint(data_);
    break;
  case PARAMETRIC:
    pixels = EllipseDrawer::Parametric(data_);
    break;
  case BRESENHAM:
    pixels = EllipseDrawer::Bresenham(data_);
    break;
  case CANONICAL:
    pixels = EllipseDrawer::Canonical(data_);
    break;
  }
  scene->drawPixels(painter, pixels);
}
