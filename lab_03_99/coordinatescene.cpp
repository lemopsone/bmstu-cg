#include "coordinatescene.h"

#include "sceneline.h"

CoordinateScene::CoordinateScene(QRectF windowRect,
                                 QRectF planeRect,
                                 bool isVisible,
                                 QColor color,
                                 QObject *parent)
    : QGraphicsScene{parent}
{
    this->setItemIndexMethod(ItemIndexMethod::NoIndex);
    this->setGridVisibility(isVisible);
    this->setGraphicsWindow(windowRect, false);
    this->setPlane(planeRect, true);
    this->originalSize = QSizeF(planeRect.width(), planeRect.height());
    this->zoomPercent = 100;
    this->color = color;
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

QColor CoordinateScene::getColor() const
{
    return color;
}

void CoordinateScene::setColor(QColor newColor)
{
    color = newColor;
}

void CoordinateScene::addLine(QLine line, LineType type, QColor color)
{
    qDebug() << line;
    addItem(new SceneLine(line, type, color, this));
    update();
}

void CoordinateScene::drawPixels(QPainter *painter, QList<Pixel> pixels)
{
    painter->setTransform(getTransform());
    for (Pixel pixel : pixels) {
        painter->setPen(pixel.color);
        QPointF adjustedPoint = toWindowCoords(pixel.point);
        painter->drawPoint(adjustedPoint);
    }
}

void CoordinateScene::setPlane(double x, double y, double w, double h, bool update)
{
    this->plane = QRectF(x, y, w, h);
    this->computeCoefficients();

    if (update) {
        this->update();
    }
}

void CoordinateScene::setPlane(QPointF topLeft, double w, double h, bool update)
{
    this->plane = QRectF(topLeft.x(), topLeft.y(), w, h);
    this->computeCoefficients();
    if (update) {
        this->update();
    }
}

void CoordinateScene::setPlane(QRectF rect, bool update)
{
    this->plane = rect;
    this->computeCoefficients();

    if (update) {
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

    if (update) {
        this->update();
    }
}

void CoordinateScene::setGraphicsWindow(QRectF rect, bool update)
{
    this->graphicsWindow = rect;
    this->setSceneRect(rect);
    if (update) {
        this->computeCoefficients();
    }
}

QRectF CoordinateScene::getGraphicsWindow(void)
{
    return this->graphicsWindow;
}

void CoordinateScene::computeCoefficients(void)
{
    if (!this->plane.isValid() || !this->graphicsWindow.isValid()) {
        return;
    }

    this->setCX(this->graphicsWindow.width() / this->plane.width());
    this->setCY(this->graphicsWindow.height() / this->plane.height());
}

QPointF CoordinateScene::toPlaneCoords(QPointF windowCoords)
{
    QPointF planeCoords = windowCoords;

    if (this->cX == 0 || this->cY == 0) {
        throw new std::runtime_error("Коэффициент равен нулю, невозможно перевести координаты");
    }

    planeCoords.setX((windowCoords.x() - this->graphicsWindow.topLeft().x()) / this->cX
                     + this->plane.topLeft().x());
    planeCoords.setY((windowCoords.y() - this->graphicsWindow.topLeft().y()) / this->cY
                     + this->plane.topLeft().y());

    return planeCoords;
}

QPointF CoordinateScene::toWindowCoords(QPointF planeCoords)
{
    QPointF windowCoords = planeCoords;
    windowCoords.setX((planeCoords.x() - this->plane.topLeft().x()) * this->cX
                      + this->graphicsWindow.topLeft().x());
    windowCoords.setY((planeCoords.y() - this->plane.topLeft().y()) * this->cY
                      + this->graphicsWindow.topLeft().y());

    return windowCoords;
}

void CoordinateScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    /*
   * Задание матрицы трансформации
   * необходимо из-за механики работы QGraphicsItem::paint(), где painter
   * получает матрицу трансформации, отличную от используемой в сцене
   */
    if (this->transform == QTransform()) {
        this->transform = painter->transform();
    }

    painter->fillRect(rect, color);

    if (!this->gridIsVisible()) {
        return;
    }

    painter->setPen(Qt::gray);
    painter->setBrush(Qt::gray);
    const int STEP_COUNT = 20;
    int step = std::min(plane.height(), plane.width()) / STEP_COUNT;
    if (step == 0)
    {
        step = 1;
    }
    else if (step > 5)
    {
        step -= step % 5;
    }
    // Отрисовка координатной сетки
    for (long i = plane.left() / step + 1; i < plane.right() / step + 1; i++)
    {
        QPointF top = this->toWindowCoords(QPointF(i * step, plane.top()));
        QPointF bottom = this->toWindowCoords(QPointF(i * step, plane.bottom()));
        painter->drawLine(top, bottom);
    }
    for (long i = plane.top() / step + 1; i < plane.bottom() / step + 1; i++)
    {
        QPointF left = this->toWindowCoords(QPointF(plane.left(), i * step));
        QPointF right = this->toWindowCoords(QPointF(plane.right(), i * step));
        painter->drawLine(left, right);
    }


    // for (long x = this->plane.left() - 1; x < this->plane.right() + 1; x++) {
    //     QPointF top = this->toWindowCoords(QPointF(x, this->plane.top()));
    //     QPointF bottom = this->toWindowCoords(QPointF(x, this->plane.bottom()));
    //     painter->drawLine(top, bottom);
    // }

    // for (long y = this->plane.top() - 1; y < this->plane.bottom() + 1; y++) {
    //     QPointF left = this->toWindowCoords(QPointF(this->plane.left(), y));
    //     QPointF right = this->toWindowCoords(QPointF(this->plane.right(), y));
    //     painter->drawLine(left, right);
    // }

    // Оси абсцисс и ординат, центр окружности
    painter->setPen(Qt::black);
    bool intersectsX = this->plane.top() <= 0 && this->plane.bottom() >= 0;
    bool intersectsY = this->plane.left() <= 0 && this->plane.right() >= 0;

    if (intersectsX) {
        this->drawXAxis(painter);
    }
    if (intersectsY) {
        this->drawYAxis(painter);
    }
    if (intersectsX && intersectsY) {
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

    if (!text.isEmpty()) {
        this->drawText(painter, convertedPoint - QPointF(rx, -rx), QSizeF(2 * rx, 2 * rx), text);
    }
}

void CoordinateScene::drawText(QPainter *painter, QPointF startingPos, QString text)
{
    QTransform transform;
    transform.translate(startingPos.x() - this->graphicsWindow.left(),
                        this->graphicsWindow.bottom() - startingPos.y());
    painter->save();
    painter->setTransform(transform);
    painter->setFont(QFont("Times New Roman"));
    painter->drawText(QPointF(0, 0), text);
    painter->restore();
}

void CoordinateScene::drawText(QPainter *painter, QPointF startingPos, QSizeF size, QString text)
{
    QTransform transform;
    transform.translate(startingPos.x() - this->graphicsWindow.left(),
                        this->graphicsWindow.bottom() - startingPos.y());
    painter->save();
    painter->setTransform(transform);
    painter->setFont(QFont("Times New Roman"));
    painter->setPen(Qt::white);
    painter->drawText(QRectF(QPointF(0, 0), size), Qt::AlignHCenter | Qt::AlignTop, text);
    painter->restore();
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
    if (this->autoScale) {
        this->autoScaleContents();
    }
}

bool CoordinateScene::getAutoScale()
{
    return this->autoScale;
}

void CoordinateScene::autoScaleContents()
{
    if (this->items().size() == 0) {
        this->setPlane(QPointF(-this->originalSize.width() / 2, -this->originalSize.height() / 2),
                       this->originalSize.width(),
                       this->originalSize.height());
        this->zoomPercent = 100;
        emit zoomChanged(100);
        return;
    }

    QRectF boundingRect = this->findItemsBoundingRect();
    QSizeF itemsSize = boundingRect.size();
    if (3 * itemsSize.width() > 4 * itemsSize.height()) {
        itemsSize.setHeight((3.0 / 4.0) * itemsSize.width());
    } else if (3 * itemsSize.width() < 4 * itemsSize.height()) {
        itemsSize.setWidth((4.0 / 3.0) * itemsSize.height());
    }
    qreal zoom = 100.0 * this->originalSize.width() / itemsSize.width();

    QPointF topLeft = boundingRect.center()
                      - QPointF(itemsSize.width() * 0.6, itemsSize.height() * 0.6);
    this->setPlane(topLeft, itemsSize.width() * 1.2, itemsSize.height() * 1.2);
    this->zoomPercent = zoom;
    emit zoomChanged(zoom);
}

QRectF CoordinateScene::findItemsBoundingRect()
{
    qreal top = INFINITY, bottom = -INFINITY, left = INFINITY, right = -INFINITY;
    for (QGraphicsItem *item : this->items()) {
        QRectF boundingRect = item->boundingRect();
        top = std::min(top, boundingRect.top());
        bottom = std::max(bottom, boundingRect.bottom());
        left = std::min(left, boundingRect.left());
        right = std::max(right, boundingRect.right());
    }

    return QRectF(left, top, right - left, bottom - top);
}

void CoordinateScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF currentPos = this->toPlaneCoords(mouseEvent->scenePos());
    emit mousePositionChanged(currentPos);

    if (mouseEvent->buttons() & Qt::MouseButton::MiddleButton) {
        QPointF translation = currentPos - this->dragStartPos;

        this->setPlane(this->plane.topLeft() - translation,
                       this->plane.width(),
                       this->plane.height());

        this->dragStartPos = currentPos - translation;
    }
}

void CoordinateScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF windowPoint = mouseEvent->scenePos();
    Qt::MouseButton buttonPressed = mouseEvent->button();
    if (buttonPressed == Qt::MouseButton::MiddleButton) {
        if (!this->isDragged) {
            this->dragStartPos = this->toPlaneCoords(windowPoint);
            this->isDragged = true;
        }
    }
}

void CoordinateScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent)
{
    double delta = wheelEvent->pixelDelta().y();
    QPointF cursorPos = wheelEvent->scenePos();
    double zoom = 1 - delta / this->graphicsWindow.width();
    double xRatio = (cursorPos.x() - this->graphicsWindow.left()) / this->graphicsWindow.width();
    double yRatio = (cursorPos.y() - this->graphicsWindow.top()) / this->graphicsWindow.height();
    QPointF newTopLeft = QPointF(cursorPos.x() - xRatio * zoom * this->graphicsWindow.width(),
                                 cursorPos.y() - yRatio * zoom * this->graphicsWindow.height());
    QPointF newTopLeftPlane = this->toPlaneCoords(newTopLeft);

    this->setPlane(newTopLeftPlane, this->plane.width() * zoom, this->plane.height() * zoom);
    this->zoomPercent /= zoom;
    emit zoomChanged(this->zoomPercent);
}

void CoordinateScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    Qt::MouseButton buttonPressed = mouseEvent->button();
    if (buttonPressed == Qt::MouseButton::MiddleButton) {
        this->isDragged = false;
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
