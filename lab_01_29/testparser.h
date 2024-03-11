#ifndef TESTPARSER_H
#define TESTPARSER_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QList>
#include "test.h"

class TestParser
{
public:
    TestParser();
    bool loadFromFile(QString filepath);
    QList<Test> tests();
protected:
    QJsonDocument doc;
};

#endif // TESTPARSER_H
