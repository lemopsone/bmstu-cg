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

void CoordinateScene::createLine(QPoint start, QPoint end) {
  QLine line(start, end);
  lines << line;
  painter->begin(image);
  painter->setPen(lineColor);
  drawLine(start, end, painter);
  painter->end();
  drawImage();
}

void CoordinateScene::createClipShape(ClipShape shape) {
  if (!clipper.shape().isEmpty()) {
    repaintLines();
  }
  clipper.setShape(shape);
  if (shape.size() >= 3) {
    clipper.setLocked(true);
  }
  painter->begin(image);
  painter->setPen(clipShapeColor);
  drawClipShape(shape, painter, clipper.locked());
  painter->end();
  drawImage();
}

void CoordinateScene::drawClipShape(ClipShape shape, QPainter *painter,
                                    bool closed) {
  for (int i = 0; i < shape.size(); i++) {
    painter->drawLine(shape.at(i), shape.at((i + 1) % shape.size()));
  }
  if (closed) {
    painter->drawLine(shape.first(), shape.last());
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

  QPoint normalized = normalizePoint(windowPoint);

  QImage tempImage(*image);
  painter->begin(&tempImage);
  if (drawMode == CLIPPER) {
    painter->setPen(clipShapeColor);
  } else {
    painter->setPen(lineColor);
  }
  painter->drawLine(prevPoint, normalized);
  painter->end();

  drawImage(&tempImage);
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

void CoordinateScene::addClipShapePoint(QPoint point) {
  if (clipper.locked()) {
    clipper.setLocked(false);
    clipper.setShape(QPolygon());
    repaintLines();
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

void CoordinateScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
  QPoint windowPoint = mouseEvent->scenePos().toPoint();
  QPoint normalized = normalizePoint(windowPoint);
  auto button = mouseEvent->button();
  if (button == Qt::RightButton && figurePoint != FIRST) {
    figurePoint = NEXT;
    if (drawMode == CLIPPER) {
      lockClipShape();
    } else {
      drawImage();
    }
  } else if (button == Qt::LeftButton) {
    if (prevPoint != normalized) {
      if (drawMode == CLIPPER) {
        addClipShapePoint(normalized);
        prevPoint = normalized;
        figurePoint = NEXT;
      } else if (drawMode == LINE) {
        if (figurePoint == FIRST) {
          prevPoint = normalized;
          figurePoint = NEXT;
        } else {
          createLine(prevPoint, normalized);
          figurePoint = FIRST;
          emit lineCreated(prevPoint, normalized);
        }
      }
    }
  }
}
