#include "testactions.h"
#include "myrectf.h"

TestActions::TestActions(QObject *parent) :
    QObject(parent)
{}

void TestActions::moveRect()
{
    MyRectF rect(
        QPointF(-2, -2),
        QPointF(-2, 2),
        QPointF(2, 2),
        QPointF(2, -2)
        );

    QList<QPointF> srcPoints = rect.getPoints();

    QPointF coordChange(5, 6);

    Move move{
        .x = coordChange.x(),
        .y = coordChange.y()
    };

    rect.move(move);
    QList<QPointF> newPoints = rect.getPoints();
    for (qsizetype i = 0; i < 4; i++)
    {
        QCOMPARE_EQ(srcPoints.at(i) + coordChange, newPoints.at(i));
    }
}

void TestActions::scaleRect()
{
    MyRectF rect(
        QPointF(-2, -2),
        QPointF(-2, 2),
        QPointF(2, 2),
        QPointF(2, -2)
        );

    QList<QPointF> srcPoints = rect.getPoints();

    Scale scale{
        .kx = 2,
        .ky = 3,
        .center = rect.center()
    };

    rect.scale(scale);
    QList<QPointF> newPoints = rect.getPoints();
    for (qsizetype i = 0; i < 4; i++)
    {
        QCOMPARE_EQ(srcPoints.at(i).x() * 2, newPoints.at(i).x());
        QCOMPARE_EQ(srcPoints.at(i).y() * 3, newPoints.at(i).y());
    }
}

void TestActions::rotateRect()
{
    MyRectF rect(
        QPointF(-2, -2),
        QPointF(-2, 2),
        QPointF(2, 2),
        QPointF(2, -2)
        );

    MyRectF expectedRect(
        QPointF(-2, 2),
        QPointF(2, 2),
        QPointF(2, -2),
        QPointF(-2, -2)
    );

    Rotate rotate{
        .angle = qDegreesToRadians(90),
        .center = rect.center()
    };

    rect.rotate(rotate);
    QList<QPointF> expectedPoints = expectedRect.getPoints();
    QList<QPointF> newPoints = rect.getPoints();
    for (qsizetype i = 0; i < 4; i++)
    {
        QCOMPARE_EQ(expectedPoints.at(i), newPoints.at(i));
    }
}

QTEST_MAIN(TestActions);
