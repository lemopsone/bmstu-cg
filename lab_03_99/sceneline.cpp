#include "sceneline.h"

#include "coordinatescene.h"

SceneLine::SceneLine(QLine line, LineType type, QColor color,
                     CoordinateScene *scene_)
    : SceneObject(scene_) {
  this->line_ = line;
  this->type_ = type;
  this->color_ = color;
  this->setPos(this->line_.center());
}

SceneLine::SceneLine(LineData data, CoordinateScene *scene_)
    : SceneObject(scene_) {
  this->line_ = data.line;
  this->type_ = data.type;
  this->color_ = data.color;
  this->setPos(this->line_.center());
}

QRectF SceneLine::boundingRect() const {
  return QRectF(this->line_.p1(), this->line_.p2()).normalized();
}

void SceneLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
  Q_UNUSED(option);
  Q_UNUSED(widget);

  // painter->setTransform(this->sceneTransform());
  QList<Pixel> pixels;
  switch (type_) {
  case DDA:
    pixels = LineDrawer::DDA(line_.p1(), line_.p2(), color_);
    break;
  case BRESENHAM_INT:
    pixels = LineDrawer::BresenhamInt(line_.p1(), line_.p2(), color_);
    break;
  case BRESENHAM_FLOAT:
    pixels = LineDrawer::Bresenham(line_.p1(), line_.p2(), color_);
    break;
  case BRESENHAM_SMOOTH:
    pixels = LineDrawer::BresenhamSmooth(line_.p1(), line_.p2(), color_,
                                         scene->getColor());
    break;
  case WU:
    pixels = LineDrawer::Wu(line_.p1(), line_.p2(), color_, scene->getColor());
    break;
  }
  scene->drawPixels(painter, pixels);
}
