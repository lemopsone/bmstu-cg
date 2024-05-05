#include "testlinedrawer.h"

TestLineDrawer::TestLineDrawer(QObject *parent) :
    QObject(parent)
{}

void TestLineDrawer::countStepsHorizontalLine()
{
    QList<Pixel> pixels = LineDrawer::DDA(QPoint(0, 0), QPoint(0, 100));
    qreal numSteps = LineDrawer::stepCount(pixels);
    Q_ASSERT(numSteps == 1);
}

void TestLineDrawer::countStepsVerticalLine()
{
    QList<Pixel> pixels = LineDrawer::DDA(QPoint(0, 0), QPoint(100, 0));
    qreal numSteps = LineDrawer::stepCount(pixels);
    Q_ASSERT(numSteps == 1);
}

QTEST_MAIN(TestLineDrawer);
