#include "scenepoint.h"
#include "coordinatescene.h"

ScenePoint::ScenePoint(QPointF coordinates, CoordinateScene *scene_) : SceneObject(scene_)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->coords = coordinates;
    this->setPos(this->coords);
    this->setToolTip(QString("(%1; %2)")
        .arg(std::round(this->coords.x() * 1000.0) / 1000.0)
        .arg(std::round(this->coords.y() * 1000.0) / 1000.0)
    );
    this->setZValue(2);
}

QRectF ScenePoint::boundingRect() const
{
    qreal diameter = this->scene->getPlane().width() * 0.02;
    QSizeF pointSize = QSizeF(diameter, diameter);
    return QRectF(this->coords - QPointF(diameter / 2, diameter / 2), pointSize);
}

void ScenePoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
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

    qsizetype idx = this->scene->getPoints().indexOf(this);
    this->scene->drawPoint(painter, this->coords, QString::number(idx));
}
