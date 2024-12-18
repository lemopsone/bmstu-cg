#ifndef _SCENECIRCLE_H_
#define _SCENECIRCLE_H_

#include "circledrawer.h"
#include "sceneobject.h"

class SceneCircle : public SceneObject {
public:
  SceneCircle(CircleData data, CoordinateScene *scene_);
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget) override;

private:
  CircleData data_;
};

#endif // _SCENECIRCLE_H_
