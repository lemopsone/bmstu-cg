#include "scenepoint.h"

ScenePoint::ScenePoint(QPointF coordinates, CoordinateScene *scene_)
{
    this->setFlag(QGraphicsItem::ItemIsFocusable, false);
    this->setFlag(QGraphicsItem::ItemIsFocusScope, false);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->scene = scene_;
    this->coords = coordinates;
    this->setPos(this->coords);
    this->setToolTip(QString("(%1; %2)")
        .arg(std::round(this->coords.x() * 1000.0) / 1000.0)
        .arg(std::round(this->coords.y() * 1000.0) / 1000.0)
    );
}

ScenePoint::~ScenePoint()
{
}

QRectF ScenePoint::boundingRect() const
{
    QPointF windowCoordsCenter = this->scene->toWindowCoords(this->coords);
    qDebug() << "window: " << this->coords << windowCoordsCenter;
    qreal diameter = this->scene->getGraphicsWindow().width() * 0.02;
    QPointF windowCoordsTopLeft = windowCoordsCenter - QPointF(diameter / 2, diameter / 2);
    QSizeF pointSize = QSizeF(diameter, diameter);
    return QRectF(windowCoordsTopLeft, pointSize);
}

void ScenePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Если точка не входит в рабочую зону, не отрисовываем
    QRectF boundingRect = this->boundingRect();
    if (!this->scene->getGraphicsWindow().intersects(boundingRect))
    {
        return;
    }
    painter->setTransform(this->scene->getTransform());
    QBrush brush = QBrush(Qt::blue);
    if (this->isSelected())
    {
        brush.setColor(Qt::green);
    }
    painter->setPen(Qt::black);
    painter->setBrush(brush);

    this->scene->drawPoint(painter, this->scene->toPlaneCoords(boundingRect.center()));
}

void ScenePoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    return;
}

void ScenePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    return;
}
