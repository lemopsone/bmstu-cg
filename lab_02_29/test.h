#ifndef TEST_H
#define TEST_H

#include <QDebug>
#include <QList>
#include <QString>

#include "actions.h"

class Test
{
public:
    Test();
    Test(QString name, QString desc, QList<Action> actions);
    QString name() const;
    QString desc() const;
    QList<Action> actions() const;

    bool isEmpty();

private:
    QString name_;
    QString desc_;
    QList<Action> actions_;
};
QDebug operator<<(QDebug stream, const Test &test);

#endif // TEST_H
