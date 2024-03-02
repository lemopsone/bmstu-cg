#include "scenepoint.h"

ScenePoint::ScenePoint(QPointF coordinates, CoordinateScene *scene_) : SceneObject(scene_)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->coords = coordinates;
    this->setPos(this->coords);
    this->setToolTip(QString("(%1; %2)")
        .arg(std::round(this->coords.x() * 1000.0) / 1000.0)
        .arg(std::round(this->coords.y() * 1000.0) / 1000.0)
    );
}

QRectF ScenePoint::boundingRect() const
{
    // QPointF windowCoordsCenter = this->scene->toWindowCoords(this->coords);
    // qreal diameter = this->scene->getGraphicsWindow().width() * 0.02;
    // QPointF windowCoordsTopLeft = windowCoordsCenter - QPointF(diameter / 2, diameter / 2);
    // QSizeF pointSize = QSizeF(diameter, diameter);
    // return QRectF(windowCoordsTopLeft, pointSize);
    return QRectF(this->coords, QSizeF(0, 0));
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

    this->scene->drawPoint(painter, this->coords);
}

