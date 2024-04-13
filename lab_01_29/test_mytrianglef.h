#ifndef TEST_MYTRIANGLEF_H
#define TEST_MYTRIANGLEF_H

#include <QTest>

class TestMyTriangleF : public QObject
{
    Q_OBJECT
public:
    explicit TestMyTriangleF(QObject *parent = 0);

private slots:
    void rightTriangle();
};

#endif // TEST_MYTRIANGLEF_H
