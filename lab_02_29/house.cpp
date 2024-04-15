#include "house.h"

#include "coordinatescene.h"

House::House(CoordinateScene *scene_) {
    this->scene = scene_;
    createRoof();
    createDoor();
    createWindow();
    createWall();
}

House::~House()
{
    while (this->objects.count()) {
        delete this->objects.takeLast();
    }
}

void House::processAction(Action action, bool push)
{
    switch (action.type) {
    case MOVE:
        this->move(action.move);
        break;
    case ROTATE:
        this->rotate(action.rotate);
        break;
    case SCALE:
        this->scale(action.scale);
        break;
    default:
        return;
    }
    if (push) {
        undo_stack.push_back(action);
        redo_stack.clear();

    }
}

void House::undo()
{
    qDebug() << "undo called";
    if (!undo_stack.empty()) {
        Action last = undo_stack.back();
        undo_stack.pop_back();
        redo_stack.push_back(last);
        Action reversed = this->inverseAction(last);
        this->processAction(reversed, false);
    }
}

void House::redo()
{
    if (!redo_stack.empty()) {
        Action next = redo_stack.back();
        redo_stack.pop_back();
        undo_stack.push_back(next);
        this->processAction(next, false);
    }
}

void House::move(Move move)
{
    for (SceneObject *object : objects) {
        object->move(move);
    }
    MyShapeF::movePoint(center, move);
}

void House::rotate(Rotate rotate)
{
    if (rotate.center == nullptr) {
        rotate.center = &this->center;
    } else {
        MyShapeF::rotatePoint(center, rotate);
    }
    for (SceneObject *object : objects) {
        object->rotate(rotate);
    }
}

void House::scale(Scale scale)
{
    if (scale.center == nullptr) {
        scale.center = &this->center;
    } else {
        MyShapeF::scalePoint(center, scale);
    }
    for (SceneObject *object : objects) {
        object->scale(scale);
    }
}

void House::createRoof()
{
    SceneTrapezium *roof = new SceneTrapezium(
        MyTrapeziumF(
            QPointF(-9, 6), QPointF(9, 6), QPointF(10, 2), QPointF(-10, 2)
            ), scene
        );
    objects << roof;
    trapeziums << roof;
}

void House::createDoor()
{
    SceneRectangle *door, *knob;
    SceneLine *a, *b;
    door = new SceneRectangle(
        MyRectF(
            QPointF(-6, -6), QPointF(-6, 0), QPointF(-3, 0), QPointF(-3, -6)
            ), scene
        );
    knob = new SceneRectangle(
        MyRectF(
            QPointF(-3.75, -2.25), QPointF(-3.25, -2.25),
            QPointF(-3.25, -3.75), QPointF(-3.75, -3.75)
            ), scene
        );
    a = new SceneLine(
        QLineF(QPointF(-6, 0), QPointF(-3, -6)), scene
        );
    b = new SceneLine(
        QLineF(QPointF(-6, -6), QPointF(-3, 0)), scene
        );

    rectangles << door << knob;
    lines << a << b;
    objects << door << knob << a << b;
}

void House::createWindow()
{
    SceneEllipse *window;
    SceneLine *vertical, *horizontal;
    window = new SceneEllipse(
        MyEllipseF(QPointF(4, -2), QPointF(2, -2), QPointF(4, 0)), scene
        );
    vertical = new SceneLine(
        QLineF(QPointF(4, 0), QPointF(4, -4)), scene
        );
    horizontal = new SceneLine(
        QLineF(QPointF(2, -2), QPointF(6, -2)), scene
        );
    ellipses << window;
    lines << vertical << horizontal;
    objects << window << vertical << horizontal;
}

void House::createWall()
{
    SceneRectangle *wall;
    wall = new SceneRectangle(
        MyRectF(
            QPointF(-8, 2), QPointF(8, 2),
            QPointF(8, -6), QPointF(-8, -6)
            ), scene
        );
    rectangles << wall;
    objects << wall;
}

Action House::inverseAction(const Action &action)
{
    Action reversed = action;
    switch (action.type) {
    case MOVE:
        reversed.move.x *= -1;
        reversed.move.y *= -1;
        break;
    case ROTATE:
        reversed.rotate.angle *= -1;
        break;
    case SCALE:
        reversed.scale.kx = 1 / action.scale.kx;
        reversed.scale.ky = 1 / action.scale.ky;
        break;
    default:
        return action;
    }

    return reversed;
}
