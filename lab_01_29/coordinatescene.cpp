#include "coordinatescene.h"

CoordinateScene::CoordinateScene(QRectF windowRect, QRectF planeRect, bool isVisible, QObject *parent)
    : QGraphicsScene{parent}
{
    this->setItemIndexMethod(ItemIndexMethod::NoIndex);
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
void CoordinateScene::drawPoint(QPainter *painter, QPointF point, QString text)
{
    QPointF convertedPoint = this->toWindowCoords(point);
    double rx = 8;
    painter->drawEllipse(convertedPoint, rx, rx);

    if (!text.isEmpty())
    {
        this->drawText(painter, convertedPoint - QPointF(rx, -rx), QSizeF(2 * rx, 2 * rx), text);
    }
}

void CoordinateScene::drawLine(QPainter *painter, QLineF line, bool showAngle)
{
    QPointF p1 = this->toWindowCoords(line.p1());
    QPointF p2 = this->toWindowCoords(line.p2());
    painter->drawLine(p1, p2);

    if (showAngle)
    {
        this->drawLineAngle(painter, QLineF(p1, p2));
    }
}

void CoordinateScene::drawMyRect(QPainter *painter, MyRectF rect)
{
    Q_ASSERT(rect.isValid());

    QList<QPointF> points = rect.getPoints();
    // Приведение точек к координатам окна
    for (short i = 0; i < 4; i++)
    {
        points[i] = this->toWindowCoords(points[i]);
    }
    // Отрисовка линий
    for (short i = 0; i < 4; i++)
    {
        painter->drawLine(QLineF(points[i % 4], points[(i + 1) % 4]));
    }

}

void CoordinateScene::drawMyTriangle(QPainter *painter, MyTriangleF triangle)
{
    Q_ASSERT(triangle.isValid());

    QList<QPointF> points = triangle.getPoints();
    for (short i = 0; i < 3; i++)
    {
        points[i] = this->toWindowCoords(points[i]);
    }
    for (short i = 0; i < 3; i++)
    {
        painter->drawLine(QLineF(points[i % 3], points[(i + 1) %3]));
    }

}

void CoordinateScene::drawEllipse(QPainter *painter, QRectF ellipse)
{
    painter->drawEllipse(
        this->toWindowCoords(ellipse.topLeft()),
        ellipse.width() * this->cX,
        ellipse.height() * this->cY
    );
}

void CoordinateScene::drawText(QPainter *painter, QPointF startingPos, QString text)
{
    QTransform transform;
    transform.translate(startingPos.x() - this->graphicsWindow.left(), this->graphicsWindow.bottom() - startingPos.y());
    painter->save();
    painter->setTransform(transform);
    painter->setFont(QFont("Times New Roman"));
    painter->drawText(QPointF(0, 0), text);
    painter->restore();
}

void CoordinateScene::drawText(QPainter *painter, QPointF startingPos, QSizeF size, QString text)
{
    QTransform transform;
    transform.translate(startingPos.x() - this->graphicsWindow.left(), this->graphicsWindow.bottom() - startingPos.y());
    painter->save();
    painter->setTransform(transform);
    painter->setFont(QFont("Times New Roman"));
    painter->setPen(Qt::white);
    painter->drawText(QRectF(QPointF(0, 0), size), Qt::AlignHCenter | Qt::AlignTop, text);
    painter->restore();
}

void CoordinateScene::drawLineAngle(QPainter *painter, QLineF line)
{
    QPointF p1 = line.p1();
    QLineF axis = QLineF(p1 - QPointF(5, 0), p1 + QPointF(5, 0));
    qreal angle = line.angleTo(axis);
    qreal antiClockwiseAngle = std::min(angle, 360.0 - angle);
    qreal clockwiseAngle = 180 - antiClockwiseAngle;
    qreal c = (angle == antiClockwiseAngle) ? 1 : -1;
    QRectF rect = QRectF (p1 - QPointF(25, 25), QSizeF(50, 50));
    if (antiClockwiseAngle < clockwiseAngle)
    {
        painter->drawArc(rect, 0, -antiClockwiseAngle * 16 * c);
        painter->drawLine(p1, p1 + QPointF(25, 0));
        this->drawText(painter, p1 + QPointF(30, 10 * c), printableAngle(antiClockwiseAngle));
    }
    else
    {
        painter->drawArc(rect, 180 * 16, clockwiseAngle * 16 * c);
        painter->drawLine(p1, p1 - QPointF(25, 0));
        this->drawText(painter, p1 + QPointF(-80, 10 * c), printableAngle(clockwiseAngle));
    }
}

QList<ScenePoint *> CoordinateScene::getPoints()
{
    return this->points;
}

void CoordinateScene::addPoint(ScenePoint *point)
{
    this->points.append(point);
    this->addItem(point);
    this->update();
}

void CoordinateScene::removePoint(ScenePoint *point)
{
    if (this->points.removeOne(point))
    {
        this->removeItem(point);
        delete point;
        point = nullptr;
        this->update();
    }
}

void CoordinateScene::removePoint(qsizetype idx)
{
    if (this->points.size() <= idx)
    {
        return;
    }
    ScenePoint *point = this->points.at(idx);
    this->removePoint(point);
}

QList<SceneRectangle *> CoordinateScene::getRectangles()
{
    return this->rectangles;
}

void CoordinateScene::addRectangle(SceneRectangle *rectangle)
{
    this->rectangles.append(rectangle);
    this->addItem(rectangle);
    this->update();
}

void CoordinateScene::removeRectangle(SceneRectangle *rectangle)
{
    if (this->rectangles.removeOne(rectangle))
    {
        this->removeItem(rectangle);
        delete rectangle;
        rectangle = nullptr;
        this->update();
    }
}

void CoordinateScene::removeRectangle(qsizetype idx)
{
    if (this->rectangles.size() <= idx)
    {
        return;
    }
    SceneRectangle *rectangle = this->rectangles.at(idx);
    this->removeRectangle(rectangle);
}

QList<SceneEllipse *> CoordinateScene::getEllipses()
{
    return this->ellipses;
}

void CoordinateScene::addEllipse(SceneEllipse *ellipse)
{
    this->ellipses.append(ellipse);
    this->addItem(ellipse);
    this->update();
}

void CoordinateScene::removeEllipse(SceneEllipse *ellipse)
{
    if (this->ellipses.removeOne(ellipse))
    {
        this->removeItem(ellipse);
        delete ellipse;
        ellipse = nullptr;
        this->update();
    }
}

void CoordinateScene::removeEllipse(qsizetype idx)
{
    if (this->ellipses.size() <= idx)
    {
        return;
    }
    SceneEllipse *ellipse = this->ellipses.at(idx);
    this->removeEllipse(ellipse);
}

QList<SceneLine *> CoordinateScene::getLines()
{
    return this->lines;
}


void CoordinateScene::addLine(SceneLine *line)
{
    this->lines.append(line);
    this->addItem(line);
    this->update();
}

void CoordinateScene::removeLine(SceneLine *line)
{
    if (this->lines.removeOne(line))
    {
        this->removeItem(line);
        delete line;
        line = nullptr;
        this->update();
    }
}

void CoordinateScene::removeLine(qsizetype idx)
{
    if (this->lines.size() <= idx)
    {
        return;
    }
    SceneLine *line = this->lines.at(idx);
    this->removeLine(line);
}

QList<SceneTriangle *> CoordinateScene::getTriangles()
{
    return this->triangles;
}
void CoordinateScene::addTriangle(SceneTriangle *triangle)
{
    this->triangles.append(triangle);
    this->addItem(triangle);
    this->update();
}

void CoordinateScene::removeTriangle(SceneTriangle *triangle)
{
    if (this->triangles.removeOne(triangle))
    {
        this->removeItem(triangle);
        delete triangle;
        triangle = nullptr;
        this->update();
    }
}

void CoordinateScene::removeTriangle(qsizetype idx)
{
    if (this->triangles.size() <= idx)
    {
        return;
    }
    SceneTriangle *triangle = this->triangles.at(idx);
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

void CoordinateScene::setAutoScale(bool state)
{
    this->autoScale = state;
    if (this->autoScale)
    {
        this->autoScaleContents();
    }
}

bool CoordinateScene::getAutoScale()
{
    return this->autoScale;
}

void CoordinateScene::autoScaleContents()
{
    if (this->items().size() == 0)
    {
        this->setPlane(
            QPointF(-this->originalSize.width() / 2,
                    -this->originalSize.height() / 2),
            this->originalSize.width(),
            this->originalSize.height()
        );
        this->zoomPercent = 100;
        emit zoomChanged(100);
        return;
    }

    QRectF boundingRect = this->findItemsBoundingRect();
    QSizeF itemsSize = boundingRect.size();
    if (3 * itemsSize.width() > 4 * itemsSize.height())
    {
        itemsSize.setHeight((3.0 / 4.0) * itemsSize.width());
    }
    else if (3 * itemsSize.width() < 4 * itemsSize.height())
    {
        itemsSize.setWidth((4.0 / 3.0) * itemsSize.height());
    }
    qreal zoom = 100.0 * this->originalSize.width() / itemsSize.width();

    QPointF topLeft = boundingRect.center() - QPointF(itemsSize.width() * 0.6, itemsSize.height() * 0.6);
    this->setPlane(topLeft, itemsSize.width() * 1.2, itemsSize.height() * 1.2);
    this->zoomPercent = zoom;
    emit zoomChanged(zoom);
}

QRectF CoordinateScene::findItemsBoundingRect()
{
    qreal top = INFINITY, bottom = -INFINITY, left = INFINITY, right = -INFINITY;
    for (QGraphicsItem *item : this->items())
    {
        QRectF boundingRect = item->boundingRect();
        top = std::min(top, boundingRect.top());
        bottom = std::max(bottom, boundingRect.bottom());
        left = std::min(left, boundingRect.left());
        right = std::max(right, boundingRect.right());
    }

    return QRectF(left, top, right - left, bottom - top);
}

void CoordinateScene::setPointSelection(qsizetype idx, bool state)
{
    if (idx < this->points.size())
    {
        this->points[idx]->setSelected(state);
    }
}

void CoordinateScene::removeItem(QGraphicsItem *item)
{
    QGraphicsScene::removeItem(item);
    emit itemsChanged();
}

void CoordinateScene::addItem(QGraphicsItem *item)
{
    QGraphicsScene::addItem(item);
    emit itemsChanged();
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
    for (qsizetype i = 0; i < this->points.size(); i++)
    {
        double curDistance = this->getDistance(this->points[i]->boundingRect().center(), clickedPoint);
        if (curDistance < pointSize && curDistance < minDistance)
        {
            minDistance = curDistance;
            index = i;
        }
    }

    if (minDistance != INFINITY)
    {
        this->removePoint(index);

        emit pointDeleted(index);
        this->update();
    }
}

double CoordinateScene::getDistance(QPointF a, QPointF b)
{
    return QLineF(a, b).length();
}

QString CoordinateScene::printableAngle(qreal angle)
{
    QString degrees = QString::number(std::lround(angle * 1000.0) / 1000.0);
    return degrees + "°";
}

void CoordinateScene::removeAllPoints(void)
{
    for (qsizetype i = this->points.size() - 1; i > -1; i--)
    {
        this->removePoint(i);
    }
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
