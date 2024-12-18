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
  lineColor = colors.lineColor;
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

void CoordinateScene::setLineColor(QColor color) { lineColor = color; }

void CoordinateScene::setClipColor(QColor color) { clipColor = color; }

void CoordinateScene::setClipShapeColor(QColor color) {
  clipShapeColor = color;
}

QImage *CoordinateScene::getImage() { return image; };
void CoordinateScene::setImage(QImage *image) { this->image = image; };

void CoordinateScene::clearScene() {
  this->lines.clear();
  this->clipper.setShape(QRect());
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
    int dx = qAbs(x - startDrawPoint.x());
    int dy = qAbs(y - startDrawPoint.y());
    if (dx > dy) {
      point.setY(startDrawPoint.y());
    } else {
      point.setX(startDrawPoint.x());
    }
  }
  return point;
}

void CoordinateScene::createLine(QPoint start, QPoint end) {
  QLine line(start, end);
  lines << line;
  painter->begin(image);
  painter->setPen(lineColor);
  drawLine(start, end, painter);
  painter->end();
  drawImage();
}

void CoordinateScene::createClipShape(QPoint start, QPoint end) {
  ClipShape shape(start, end);
  if (!clipper.shape().isNull()) {
    repaintLines();
  }
  clipper.setShape(shape);
  painter->begin(image);
  painter->setPen(clipShapeColor);
  drawClipShape(start, end, painter);
  painter->end();
  drawImage();
}

void CoordinateScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  emit mousePositionChanged(mouseEvent->scenePos());
  if (figurePoint == FIRST) {
    return;
  }

  QImage tempImage = QImage(*image);
  painter->begin(&tempImage);
  painter->setPen(drawMode == LINE ? lineColor : clipShapeColor);
  QPoint newPoint = mouseEvent->scenePos().toPoint();
  if (drawMode == LINE) {
    newPoint = normalizePoint(newPoint);
    drawLine(startDrawPoint, newPoint, painter);
  } else {
    drawClipShape(startDrawPoint, newPoint, painter);
  }
  drawImage(&tempImage);
  painter->end();
  update();
}

void CoordinateScene::repaintLines() {
  image->fill(color);
  painter->begin(image);
  painter->setPen(lineColor);
  for (QLine line : lines) {
    drawLine(line.p1(), line.p2(), painter);
  }
  painter->end();
  drawImage();
}

void CoordinateScene::drawLine(QPoint start, QPoint end, QPainter *painter,
                               bool clipped) {
  painter->setPen(clipped ? clipColor : lineColor);
  painter->drawLine(start, end);
}

void CoordinateScene::drawClipShape(QPoint start, QPoint end,
                                    QPainter *painter) {
  painter->setPen(clipShapeColor);
  painter->drawRect(QRect(start, end));
}

void CoordinateScene::setDrawMode(DrawMode mode) { drawMode = mode; }

DrawMode CoordinateScene::getDrawMode() { return drawMode; }

void CoordinateScene::clipLines() {
  QList<QLine> clippedLines = clipper.clipMultiple(lines);
  for (QLine line : clippedLines) {
    if (line.isNull()) {
      continue;
    }
    painter->begin(image);
    drawLine(line.p1(), line.p2(), painter, true);
    painter->end();
  }
  drawImage();
}

void CoordinateScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  QPoint windowPoint = mouseEvent->scenePos().toPoint();
  QPoint point = windowPoint;
  Qt::MouseButton buttonPressed = mouseEvent->button();
  if (buttonPressed == Qt::MouseButton::LeftButton) {
    if (startDrawPoint == windowPoint) {
      return;
    }
    if (figurePoint == SECOND) {
      if (drawMode == LINE) {
        point = normalizePoint(windowPoint);
      }
      if (drawMode == LINE) {
        createLine(startDrawPoint, point);
        emit lineCreated(startDrawPoint, point);
      } else {
        createClipShape(startDrawPoint, point);
      }
      figurePoint = FIRST;
    } else {
      startDrawPoint = windowPoint;
      figurePoint = SECOND;
    }
  } else if (buttonPressed == Qt::MouseButton::RightButton) {
    if (figurePoint == SECOND) {
      figurePoint = FIRST;
    }
  }
}
