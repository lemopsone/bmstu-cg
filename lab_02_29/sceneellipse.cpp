#include "sceneellipse.h"

#include "coordinatescene.h"

SceneEllipse::SceneEllipse(MyEllipseF ellipse, CoordinateScene *scene_)
    : SceneObject(scene_)
{
    this->_ellipse = ellipse;
}

QRectF SceneEllipse::boundingRect() const
{
    return this->_ellipse.boundingRect();
}

void SceneEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setTransform(this->scene->getTransform());
    QPen pen = QPen(Qt::magenta);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    this->scene->drawEllipse(painter, _ellipse);
}

void SceneEllipse::move(Move move)
{
    this->_ellipse.move(move);
}

void SceneEllipse::rotate(Rotate rotate)
{
    this->_ellipse.rotate(rotate);
}

void SceneEllipse::scale(Scale scale)
{
    this->_ellipse.scale(scale);
}
