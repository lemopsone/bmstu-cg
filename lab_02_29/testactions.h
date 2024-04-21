#ifndef TESTACTIONS_H
#define TESTACTIONS_H

#include <QObject>
#include <QTest>

class TestActions : public QObject
{
    Q_OBJECT
public:
    explicit TestActions(QObject *parent = 0);
private slots:
    void moveRect();
    void scaleRect();
    void rotateRect();
};

#endif // TESTACTIONS_H
