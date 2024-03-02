#include "coordinatescene.h"

CoordinateScene::CoordinateScene(QRectF windowRect, QRectF planeRect, bool isVisible, QObject *parent)
    : QGraphicsScene{parent}
{
    // std::cout<< "constructor" << std::endl;
    this->setGridVisibility(isVisible);
    this->setGraphicsWindow(windowRect, false);
    this->setPlane(planeRect, true);
    this->originalSize = QSizeF(planeRect.width(), planeRect.height());
    this->zoomPercent = 100;
    this->transform = QTransform();
}

void CoordinateScene::setCX(double c)
{
    this->cX = c;
}

void CoordinateScene::setCY(double c)
{
    this->cY = c;
}

double CoordinateScene::getCX(void)
{
    return this->cX;
}

double CoordinateScene::getCY(void)
{
    return this->cY;
}

void CoordinateScene::setGridVisibility(bool value)
{
    gridVisible = value;
    this->update();
}

bool CoordinateScene::gridIsVisible(void)
{
    return gridVisible;
}

void CoordinateScene::setPlane(double x, double y, double w, double h, bool update)
{
    this->plane = QRectF(x, y, w, h);
    this->computeCoefficients();

    if (update)
    {
        this->update();
    }
}

void CoordinateScene::setPlane(QPointF topLeft, double w, double h, bool update)
{
    this->plane = QRectF(topLeft.x(), topLeft.y(), w, h);
    this->computeCoefficients();

    if (update)
    {
        this->update();
    }
}

void CoordinateScene::setPlane(QRectF rect, bool update)
{
    this->plane = rect;
    this->computeCoefficients();

    if (update)
    {
        this->update();
    }
}

QRectF CoordinateScene::getPlane(void)
{
    return this->plane;
}

void CoordinateScene::setGraphicsWindow(double x, double y, double w, double h, bool update)
{
    this->graphicsWindow = QRectF(x, y, w, h);
    this->computeCoefficients();

    if (update)
    {
        this->update();
    }
}

void CoordinateScene::setGraphicsWindow(QRectF rect, bool update)
{
    this->graphicsWindow = rect;
    this->setSceneRect(rect);
    if (update)
    {
        this->computeCoefficients();
    }
}

QRectF CoordinateScene::getGraphicsWindow(void)
{
    return this->graphicsWindow;
}

void CoordinateScene::computeCoefficients(void)
{
    if (!this->plane.isValid() || !this->graphicsWindow.isValid())
    {
        return;
    }

    this->setCX(this->graphicsWindow.width() / this->plane.width());
    this->setCY(this->graphicsWindow.height() / this->plane.height());
}

QPointF CoordinateScene::toPlaneCoords(QPointF windowCoords)
{
    QPointF planeCoords = windowCoords;

    if (this->cX == 0 || this->cY == 0)
    {
        throw new std::runtime_error("Коэффициент равен нулю, невозможно перевести координаты");
    }

    planeCoords.setX((windowCoords.x() - this->graphicsWindow.topLeft().x()) / this->cX + this->plane.topLeft().x());
    planeCoords.setY((windowCoords.y() - this->graphicsWindow.topLeft().y()) / this->cY + this->plane.topLeft().y());

    return planeCoords;
}

QPointF CoordinateScene::toWindowCoords(QPointF planeCoords)
{
    QPointF windowCoords = planeCoords;
    windowCoords.setX((planeCoords.x() - this->plane.topLeft().x()) * this->cX + this->graphicsWindow.topLeft().x());
    windowCoords.setY((planeCoords.y() - this->plane.topLeft().y()) * this->cY + this->graphicsWindow.topLeft().y());

    return windowCoords;
}

void CoordinateScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    /*
     * Задание матрицы трансформации
     * необходимо из-за механики работы QGraphicsItem::paint(), где painter
     * получает матрицу трансформации, отличную от используемой в сцене
     */
    if (this->transform == QTransform())
    {
        this->transform = painter->transform();
    }

    // Белый фон
    painter->fillRect(rect, Qt::white);

    if (!this->gridIsVisible())
    {
        return;
    }

    painter->setPen(Qt::gray);
    painter->setBrush(Qt::gray);
    // Отрисовка координатной сетки
    for (long x = this->plane.left() - 1; x < this->plane.right() + 1; x++)
    {
        QPointF top = this->toWindowCoords(QPointF(x, this->plane.top()));
        QPointF bottom = this->toWindowCoords(QPointF(x, this->plane.bottom()));
        painter->drawLine(top, bottom);
    }

    for (long y = this->plane.top() - 1; y < this->plane.bottom() + 1; y++)
    {
        QPointF left = this->toWindowCoords(QPointF(this->plane.left(), y));
        QPointF right = this->toWindowCoords(QPointF(this->plane.right(), y));
        painter->drawLine(left, right);
    }

    // Оси абсцисс и ординат, центр окружности
    painter->setPen(Qt::black);
    bool intersectsX = this->plane.top() <= 0 && this->plane.bottom() >= 0;
    bool intersectsY = this->plane.left() <= 0 && this->plane.right() >= 0;

    if (intersectsX)
    {
        this->drawXAxis(painter);
    }
    if (intersectsY)
    {
        this->drawYAxis(painter);
    }
    if (intersectsX && intersectsY)
    {
        this->drawCenter(painter);
    }
}

void CoordinateScene::drawXAxis(QPainter *painter)
{
    painter->setPen(Qt::black);
    QPointF xAxisStart = this->toWindowCoords(QPointF(this->plane.left(), 0));
    QPointF xAxisEnd = this->toWindowCoords(QPointF(this->plane.right(), 0));
    painter->drawLine(xAxisStart, xAxisEnd);
}

void CoordinateScene::drawYAxis(QPainter *painter)
{
    painter->setPen(Qt::black);
    QPointF yAxisStart = this->toWindowCoords(QPointF(0, this->plane.top()));
    QPointF yAxisEnd = this->toWindowCoords(QPointF(0, this->plane.bottom()));
    painter->drawLine(yAxisStart, yAxisEnd);
}

void CoordinateScene::drawCenter(QPainter *painter)
{
    painter->setPen(Qt::red);
    painter->setBrush(Qt::red);
    QPointF center = QPointF(0, 0);

    this->drawPoint(painter, center);
}

/**
 * @brief Draws a point on coordinate plane
 * @param painter doesn't change pen/brush color itself
 * @param point plane coordinates
 */
void CoordinateScene::drawPoint(QPainter *painter, QPointF point)
{
    QPointF convertedPoint = this->toWindowCoords(point);
    double rx = this->graphicsWindow.width() * 0.01;
    double ry = this->graphicsWindow.height() * 0.01;
    painter->drawEllipse(convertedPoint, rx, ry);
}

void CoordinateScene::addPoint(QGraphicsItem *point)
{
    this->points.append(point);
    this->addItem(point);
}

void CoordinateScene::removePoint(QGraphicsItem *point)
{
    if (this->points.removeOne(point))
    {
        this->removeItem(point);
        delete point;
    }
}

void CoordinateScene::removePoint(qsizetype idx)
{
    if (this->points.size() <= idx)
    {
        return;
    }
    QGraphicsItem *point = this->points.at(idx);
    this->removePoint(point);
}

void CoordinateScene::addRectangle(QGraphicsItem *rectangle)
{
    this->rectangles.append(rectangle);
    this->addItem(rectangle);
}

void CoordinateScene::removeRectangle(QGraphicsItem *rectangle)
{
    if (this->rectangles.removeOne(rectangle))
    {
        this->removeItem(rectangle);
        delete rectangle;
    }
}

void CoordinateScene::removeRectangle(qsizetype idx)
{
    if (this->rectangles.size() <= idx)
    {
        return;
    }
    QGraphicsItem *rectangle = this->rectangles.at(idx);
    this->removeRectangle(rectangle);
}

void CoordinateScene::addEllipse(QGraphicsItem *ellipse)
{
    this->ellipses.append(ellipse);
    this->addItem(ellipse);
}

void CoordinateScene::removeEllipse(QGraphicsItem *ellipse)
{
    if (this->ellipses.removeOne(ellipse))
    {
        this->removeItem(ellipse);
        delete ellipse;
    }
}

void CoordinateScene::removeEllipse(qsizetype idx)
{
    if (this->ellipses.size() <= idx)
    {
        return;
    }
    QGraphicsItem *ellipse = this->ellipses.at(idx);
    this->removeEllipse(ellipse);
}


void CoordinateScene::addLine(QGraphicsItem *line)
{
    this->lines.append(line);
    this->addItem(line);
}

void CoordinateScene::removeLine(QGraphicsItem *line)
{
    if (this->lines.removeOne(line))
    {
        this->removeItem(line);
        delete line;
    }
}

void CoordinateScene::removeLine(qsizetype idx)
{
    if (this->lines.size() <= idx)
    {
        return;
    }
    QGraphicsItem *line = this->lines.at(idx);
    this->removeLine(line);
}
void CoordinateScene::addTriangle(QGraphicsItem *triangle)
{
    this->triangles.append(triangle);
    this->addItem(triangle);
}

void CoordinateScene::removeTriangle(QGraphicsItem *triangle)
{
    if (this->triangles.removeOne(triangle))
    {
        this->removeItem(triangle);
        delete triangle;
    }
}

void CoordinateScene::removeTriangle(qsizetype idx)
{
    if (this->triangles.size() <= idx)
    {
        return;
    }
    QGraphicsItem *triangle = this->triangles.at(idx);
    this->removeTriangle(triangle);
}

void CoordinateScene::setTransform(const QTransform transform)
{
    this->transform = transform;
}

QTransform CoordinateScene::getTransform() const
{
    return this->transform;
}

void CoordinateScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF windowPoint = mouseEvent->scenePos();
    Qt::MouseButton buttonPressed = mouseEvent->button();
    if (buttonPressed == Qt::MouseButton::LeftButton)
    {
        emit sceneClicked(this->toPlaneCoords(windowPoint));
    }
    else if (buttonPressed == Qt::MouseButton::MiddleButton)
    {
        if (!this->isDragged)
        {
            this->dragStartPos = this->toPlaneCoords(windowPoint);
            this->isDragged = true;
        }
    }
    else if (buttonPressed == Qt::MouseButton::RightButton)
    {
        this->removeIntersectingPoint(this->toPlaneCoords(windowPoint));
    }
}

void CoordinateScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF currentPos = this->toPlaneCoords(mouseEvent->scenePos());
    emit mousePositionChanged(currentPos);

    if (mouseEvent->buttons() & Qt::MouseButton::MiddleButton)
    {
        QPointF translation = currentPos - this->dragStartPos;

        this->setPlane(
            this->plane.topLeft() - translation,
            this->plane.width(),
            this->plane.height()
        );

        this->dragStartPos = currentPos - translation;
    }
}

void CoordinateScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
    double delta = wheelEvent->pixelDelta().y();
    QPointF cursorPos = wheelEvent->scenePos();
    double zoom = 1 - delta / this->graphicsWindow.width();
    double xRatio = (cursorPos.x() - this->graphicsWindow.left()) / this->graphicsWindow.width();
    double yRatio = (cursorPos.y() - this->graphicsWindow.top()) / this->graphicsWindow.height();
    QPointF newTopLeft = QPointF(
        cursorPos.x() - xRatio * zoom * this->graphicsWindow.width(),
        cursorPos.y() - yRatio * zoom * this->graphicsWindow.height()
    );
    QPointF newTopLeftPlane = this->toPlaneCoords(newTopLeft);

    this->setPlane(
        newTopLeftPlane,
        this->plane.width() * zoom, this->plane.height() * zoom
    );
    this->zoomPercent /= zoom;
    emit zoomChanged(this->zoomPercent);
}

void CoordinateScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Qt::MouseButton buttonPressed = mouseEvent->button();
    if (buttonPressed == Qt::MouseButton::MiddleButton)
    {
        this->isDragged = false;
    }
}

void CoordinateScene::removeIntersectingPoint(QPointF clickedPoint)
{
    int index = -1;
    double minDistance = INFINITY;
    double pointSize = this->graphicsWindow.height() * 0.02 / this->cX;
    for (qsizetype i = 0; i < this->items().size(); i++)
    {
        double curDistance = this->getDistance(this->toPlaneCoords(this->items().at(i)->boundingRect().center()), clickedPoint);
        if (curDistance < pointSize && curDistance < minDistance)
        {
            minDistance = curDistance;
            index = i;
        }
    }

    if (minDistance != INFINITY)
    {
        delete this->items().at(index);

        emit pointDeleted(index);
        this->update();
    }
}

double CoordinateScene::getDistance(QPointF a, QPointF b)
{
    return QLineF(a, b).length();
}

void CoordinateScene::removeAllPoints(void)
{
    qDeleteAll(this->items());
    this->update();
}

void CoordinateScene::zoomAtPercent(double zoom)
{
    QSizeF newSize = this->originalSize / (zoom / 100);
    QPointF oldSize = QPointF(this->plane.width(), this->plane.height());
    QPointF center = this->plane.topLeft() + (oldSize / 2);
    QPointF translate = QPointF(newSize.width() / 2, newSize.height() / 2);
    QPointF topLeft = center - translate;

    this->zoomPercent = zoom;
    this->setPlane(topLeft, newSize.width(), newSize.height());
}
