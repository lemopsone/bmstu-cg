#ifndef ACTIONS_H
#define ACTIONS_H

#include <QPointF>

enum ActionType { SCALE, ROTATE, MOVE };

struct Scale {
    qreal kx, ky;
    QPointF *center = nullptr;
};

struct Rotate {
    qreal angle;
    QPointF *center = nullptr;
};

struct Move {
    qreal x, y;
};

struct Action {
    ActionType type;
    union {
        Scale scale;
        Rotate rotate;
        Move move;
    };
};

#endif // ACTIONS_H
