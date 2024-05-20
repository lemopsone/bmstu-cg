#include "fill.h"
#include <qhash.h>
#include <qlogging.h>

bool Edge::isHorizontal() const { return start.y() == end.y(); }

bool Edge::isVertical() const { return start.x() == end.x(); }

QPoint Fill::topLeft(QList<QPoint> points) {
  QPoint topLeft = points.first();
  for (QPoint point : points) {
    if (point.x() < topLeft.x()) {
      topLeft.setX(point.x());
    }
    if (point.y() < topLeft.y()) {
      topLeft.setY(point.y());
    }
  }
  return topLeft;
}

QPoint Fill::bottomRight(QList<QPoint> points) {
  QPoint bottomRight = points.first();
  for (QPoint point : points) {
    if (point.x() > bottomRight.x()) {
      bottomRight.setX(point.x());
    }
    if (point.y() > bottomRight.y()) {
      bottomRight.setY(point.y());
    }
  }
  return bottomRight;
}

int Fill::septum(QList<QPoint> points) {
  QPoint topLeft = Fill::topLeft(points);
  QPoint bottomRight = Fill::bottomRight(points);

  int middleX = (topLeft.x() + bottomRight.x()) / 2;
  int minDst = middleX - topLeft.x();
  int curDst = 0;
  int septum = -1;

  for (const auto &point : points) {
    curDst = qAbs(point.x() - middleX);
    if (curDst <= minDst) {
      minDst = curDst;
      septum = point.x();
    }
  }

  return septum;
}

QList<Edge> Fill::createEdges(QList<QPoint> points) {
  QList<Edge> edges;
  for (int i = 0; i < points.size(); i++) {
    QPoint start = points[i];
    QPoint end = points[(i + 1) % points.size()];
    edges.append(Edge{start, end});
  }
  return edges;
}

QList<PixelRow> Fill::create(QList<QPoint> points, QColor color) {
  QHash<QPoint, QColor> pixelsMap;
  QList<PixelRow> pixels;
  QList<Edge> edges = Fill::createEdges(points);
  int septum = Fill::septum(points);

  for (const auto &edge : edges) {
    if (!edge.isHorizontal()) {
      int topY = edge.start.y();
      int bottomY = edge.end.y();
      int leftX = edge.start.x();
      int rightX = edge.end.x();
      if (topY > bottomY) {
        std::swap(topY, bottomY);
        std::swap(leftX, rightX);
      }
      double dX = (double)((rightX - leftX)) / (bottomY - topY);
      double edgeX = leftX;
      for (int y = topY; y < bottomY; y++, edgeX += dX) {
        double startX = edgeX + 1, endX = septum;
        if (septum < edgeX) {
          startX = septum;
          endX = edgeX;
        }
        PixelRow pixelRow;

        for (int x = std::round(startX); x < endX; x++) {
          QColor pixelColor = color;
          if (pixelsMap.contains(QPoint(x, y))) {
            if (pixelsMap[QPoint(x, y)] == color) {
              pixelColor = Qt::white;
            }
          }
          pixelsMap.insert(QPoint(x, y), pixelColor);
          pixelRow.append(Pixel{QPoint(x, y), pixelColor});
        }
        pixels.append(pixelRow);
      }
    }
  }

  return pixels;
}
