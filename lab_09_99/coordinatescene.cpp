#include "coordinatescene.h"
#include <QTime>
#include <qcoreapplication.h>
#include <qnamespace.h>

CoordinateScene::CoordinateScene(QRectF sceneRect, SceneColors colors,
                                 QObject *parent)
    : QGraphicsScene{parent} {
  this->setItemIndexMethod(ItemIndexMethod::NoIndex);
  this->setSceneRect(sceneRect);
  this->image = new QImage(sceneRect.size().toSize(), QImage::Format_ARGB32);
  this->painter = new QPainter();
  polygonColor = colors.polygonColor;
  clipColor = colors.clipColor;
  clipShapeColor = colors.clipShapeColor;
  color = colors.bgColor;
  image->fill(color);
  drawImage();
}

CoordinateScene::~CoordinateScene() {
  delete image;
  delete painter;
}

void CoordinateScene::setBackgroundColor(QColor color) { this->color = color; }

void CoordinateScene::setPolygonColor(QColor color) { polygonColor = color; }

void CoordinateScene::setClipColor(QColor color) { clipColor = color; }

void CoordinateScene::setClipShapeColor(QColor color) {
  clipShapeColor = color;
}

QImage *CoordinateScene::getImage() { return image; };
void CoordinateScene::setImage(QImage *image) { this->image = image; };

void CoordinateScene::clearScene() {
  this->polygons.clear();
  this->clipper.setShape(QPolygon());
  this->clipper.setLocked(false);
  image->fill(color);
  drawImage();
  this->update();
}

void CoordinateScene::drawImage(QImage *image) {
  if (image == nullptr) {
    image = this->image;
  }
  this->clear();
  this->addPixmap(QPixmap::fromImage(*image));
  update();
}

void CoordinateScene::keyPressEvent(QKeyEvent *keyEvent) {
  if (keyEvent->key() == Qt::Key_Shift) {
    parallelMode = true;
  }
}

void CoordinateScene::keyReleaseEvent(QKeyEvent *keyEvent) {
  if (keyEvent->key() == Qt::Key_Shift) {
    parallelMode = false;
  }
}

QPoint CoordinateScene::normalizePoint(QPoint point) {
  if (parallelMode) {
    int x = point.x();
    int y = point.y();
    int dx = qAbs(x - prevPoint.x());
    int dy = qAbs(y - prevPoint.y());
    if (dx > dy) {
      point.setY(prevPoint.y());
    } else {
      point.setX(prevPoint.x());
    }
  }
  return point;
}

void CoordinateScene::createPolygon(QPolygon polygon) {
  polygons << polygon;
  painter->begin(image);
  painter->setPen(polygonColor);
  drawPolygon(polygon, painter, true);
  painter->end();
  drawImage();
}

void CoordinateScene::createClipShape(ClipShape shape) {
  if (!clipper.shape().isEmpty()) {
    repaintPolygons();
  }
  clipper.setShape(shape);
  if (shape.size() >= 3) {
    clipper.setLocked(true);
  }
  painter->begin(image);
  painter->setPen(clipShapeColor);
  drawPolygon(shape, painter, clipper.locked());
  painter->end();
  drawImage();
}

void CoordinateScene::drawPolygon(QPolygon polygon, QPainter *painter,
                                  bool clipped) {
  for (int i = 1; i < polygon.size(); i++) {
    painter->drawLine(polygon.at(i - 1), polygon.at(i));
  }
  if (clipped) {
    painter->drawLine(polygon.first(), polygon.last());
  }
}

void CoordinateScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  emit mousePositionChanged(mouseEvent->scenePos());

  QPoint windowPoint = mouseEvent->scenePos().toPoint();
  if (figurePoint == FIRST) {
    return;
  }

  if (drawMode == CLIPPER && clipper.locked()) {
    return;
  }
  if (drawMode == POLYGON && currentPolygon.isEmpty()) {
    return;
  }

  QPoint normalized = normalizePoint(windowPoint);

  QImage tempImage(*image);
  painter->begin(&tempImage);
  if (drawMode == CLIPPER) {
    painter->setPen(clipShapeColor);
  } else {
    painter->setPen(polygonColor);
  }
  painter->drawLine(prevPoint, normalized);
  painter->end();

  drawImage(&tempImage);
}

void CoordinateScene::repaintPolygons() {
  image->fill(color);
  painter->begin(image);
  for (QPolygon polygon : polygons) {
    painter->setPen(polygonColor);
    drawPolygon(polygon, painter, true);
  }
  painter->end();
  drawImage();
}

void CoordinateScene::setDrawMode(DrawMode mode) { drawMode = mode; }

DrawMode CoordinateScene::getDrawMode() { return drawMode; }

void CoordinateScene::clipPolygons() {
  QList<QPolygon> clippedPolygons = clipper.clipMultiple(polygons);
  for (QPolygon polygon : clippedPolygons) {
    if (polygon.isEmpty()) {
      continue;
    }
    painter->begin(image);
    painter->setPen(clipColor);
    drawPolygon(polygon, painter, true);
    painter->setPen(polygonColor);
    painter->end();
  }
  drawImage();
}

void CoordinateScene::addClipShapePoint(QPoint point) {
  if (clipper.locked()) {
    clipper.setLocked(false);
    clipper.setShape(QPolygon());
    repaintPolygons();
    emit clipShapeCleared();
  }
  clipper.addPoint(point);
  painter->begin(image);
  painter->setPen(clipShapeColor);
  if (clipper.shape().size() > 1) {
    painter->drawLine(clipper.shape().at(clipper.shape().size() - 2), point);
  } else {
    painter->drawPoint(point);
  }
  painter->end();
  drawImage();
  emit clipShapePointAdded(point);
}

void CoordinateScene::lockClipShape() {
  if (clipper.shape().size() < 3) {
    return;
  }
  figurePoint = FIRST;
  clipper.setLocked(true);
  painter->begin(image);
  painter->setPen(clipShapeColor);
  painter->drawLine(clipper.shape().first(), clipper.shape().last());
  painter->end();
  emit clipShapeLockToggled(true);
  drawImage();
}

void CoordinateScene::addPolygonPoint(QPoint point) {
  currentPolygon << point;
  if (currentPolygon.size() > 1) {
    painter->begin(image);
    painter->setPen(polygonColor);
    drawPolygon(currentPolygon, painter, false);
    painter->end();
    drawImage();
  }
  emit polygonPointAdded(point);
}

void CoordinateScene::lockPolygon() {
  if (currentPolygon.size() < 3) {
    return;
  }
  figurePoint = FIRST;
  polygons << currentPolygon;
  painter->begin(image);
  painter->setPen(polygonColor);
  painter->drawLine(currentPolygon.first(), currentPolygon.last());
  painter->end();
  currentPolygon = QPolygon();
  emit polygonLocked();
  drawImage();
}

void CoordinateScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  QPoint windowPoint = mouseEvent->scenePos().toPoint();
  QPoint normalized = normalizePoint(windowPoint);
  auto button = mouseEvent->button();
  if (button == Qt::RightButton && figurePoint != FIRST) {
    if (drawMode == CLIPPER) {
      lockClipShape();
    } else {
      lockPolygon();
    }
  } else if (button == Qt::LeftButton) {
    if (prevPoint != normalized) {
      if (drawMode == CLIPPER) {
        addClipShapePoint(normalized);
      } else {
        addPolygonPoint(normalized);
      }
      prevPoint = normalized;
      figurePoint = NEXT;
    }
  }
}
