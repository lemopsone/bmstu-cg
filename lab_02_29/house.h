#ifndef HOUSE_H
#define HOUSE_H

#include "sceneobject.h"
#include "sceneline.h"
#include "sceneellipse.h"
#include "scenetrapezium.h"
#include "sceneline.h"
#include "scenerectangle.h"

class CoordinateScene;

class House
{
public:
    House(CoordinateScene *scene_);
    ~House();

    QVector<SceneLine *> lines;
    QVector<SceneEllipse *> ellipses;
    QVector<SceneTrapezium *> trapeziums;
    QVector<SceneRectangle *> rectangles;
    QVector<SceneObject *> objects;

    QPointF center = QPointF(0, 0);

    void processAction(Action action, bool push = true);
    void undo();
    void redo();
    void move(Move move);
    void rotate(Rotate rotate);
    void scale(Scale scale);

    void createRoof();
    void createDoor();
    void createWindow();
    void createWall();

    std::vector<Action> undo_stack;
    std::vector<Action> redo_stack;

private:
    CoordinateScene *scene;


    Action inverseAction(Action &action);

};

#endif // HOUSE_H
