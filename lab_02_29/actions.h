#ifndef ACTIONS_H
#define ACTIONS_H

#include <QPointF>

enum ActionType { SCALE, ROTATE, MOVE };

struct Scale {
    qreal kx, ky;
    std::optional<QPointF> center = std::nullopt;
};

struct Rotate {
    qreal angle;
    std::optional<QPointF> center = std::nullopt;
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
