#include "mytrianglef.h"
#include "test_mytrianglef.h"

TestMyTriangleF::TestMyTriangleF(QObject *parent) :
    QObject(parent)
{}

void TestMyTriangleF::rightTriangle(void)
{
    QPointF a(0, 0), b(3, 0), c(0, 4);
    MyTriangleF triangle(a, b, c);
    QCOMPARE(triangle.isValid(), true);
    QCOMPARE(triangle.bisectIntersection(), QPointF(1, 1));
}

QTEST_MAIN(TestMyTriangleF);