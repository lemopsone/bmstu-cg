#include "coordinatescene.h"
#include <QTime>
#include <qcoreapplication.h>
#include <qnamespace.h>

CoordinateScene::CoordinateScene(QRectF sceneRect, QColor color,
                                 QObject *parent)
    : QGraphicsScene{parent} {
  this->setItemIndexMethod(ItemIndexMethod::NoIndex);
  this->setSceneRect(sceneRect);
  this->edgeColor = color;
  this->image = new QImage(sceneRect.size().toSize(), QImage::Format_ARGB32);
  this->painter = new QPainter();
  image->fill(Qt::white);
  drawImage();
}

CoordinateScene::~CoordinateScene() {
  delete image;
  delete painter;
}

QImage *CoordinateScene::getImage() { return image; };
void CoordinateScene::setImage(QImage *image) { this->image = image; };

QColor CoordinateScene::getEdgeColor() const { return edgeColor; }

void CoordinateScene::setEdgeColor(QColor newColor) {
  edgeColor = newColor;
  update();
}

bool CoordinateScene::isLocked() { return this->polygonLocked; }

void CoordinateScene::setLocked(bool state) {
  if (state) {
    if (this->polygonPoints.size() < 3) {
      return;
    }
    painter->begin(image);
    painter->drawLine(polygonPoints.last(), polygonPoints.first());
    painter->end();
    drawImage();
  }
  this->polygonLocked = state;
}

void CoordinateScene::clearScene() {
  this->polygonPoints.clear();
  this->setLocked(false);
  image->fill(Qt::white);
  drawImage();
  this->update();
}

void CoordinateScene::addPolygonPoint(QPoint point) {
  this->polygonPoints.append(point);
  painter->begin(image);
  painter->setPen(edgeColor);
  if (polygonPoints.size() > 1) {
    painter->drawLine(polygonPoints[polygonPoints.size() - 2],
                      polygonPoints[polygonPoints.size() - 1]);
  } else {
    painter->drawPoint(point);
  }
  painter->end();
  drawImage();
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
  if (polygonPoints.size() > 0) {
    if (parallelMode) {
      int x = point.x();
      int y = point.y();
      int dx = qAbs(x - polygonPoints.last().x());
      int dy = qAbs(y - polygonPoints.last().y());
      if (dx > dy) {
        point.setY(polygonPoints.last().y());
      } else {
        point.setX(polygonPoints.last().x());
      }
    }
  }
  return point;
}

void CoordinateScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  emit mousePositionChanged(mouseEvent->scenePos());
  if (polygonPoints.empty()) {
    return;
  }

  if (polygonLocked) {
    return;
  }
  QImage tempImage = QImage(*image);
  painter->begin(&tempImage);
  painter->setPen(edgeColor);
  QPoint newPoint = normalizePoint(mouseEvent->scenePos().toPoint());
  painter->drawLine(polygonPoints.last(), newPoint);
  drawImage(&tempImage);
  painter->end();
  update();
}

void CoordinateScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  QPoint windowPoint = mouseEvent->scenePos().toPoint();
  Qt::MouseButton buttonPressed = mouseEvent->button();
  if (buttonPressed == Qt::MouseButton::LeftButton) {
    QPoint normalizedPoint = this->normalizePoint(windowPoint);
    addPolygonPoint(normalizedPoint);
    emit sceneClicked(normalizedPoint);
  } else if (buttonPressed == Qt::MouseButton::RightButton) {
    setLocked(true);
    emit figureLocked();
  } else if (buttonPressed == Qt::MouseButton::MiddleButton) {
    emit seedPointSet(windowPoint);
  }
}
