#ifndef TEST_H
#define TEST_H

#include <QDebug>
#include <QList>
#include <QString>



class Test
{
public:
    Test();
    Test(QString name, QString desc);
    QString name() const;
    QString desc() const;

    bool isEmpty();

private:
    QString name_;
    QString desc_;
};
QDebug operator<<(QDebug stream, const Test &test);

#endif // TEST_H
