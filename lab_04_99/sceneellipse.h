#ifndef SCENEELLIPSE_H
#define SCENEELLIPSE_H

#include "ellipsedrawer.h"
#include "sceneobject.h"

class SceneEllipse : public SceneObject {
public:
  SceneEllipse(EllipseData data, CoordinateScene *scene_);
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

private:
  EllipseData data_;
};

#endif // SCENEELLIPSE_H
