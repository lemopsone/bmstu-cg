#ifndef TESTPARSER_H
#define TESTPARSER_H

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

#include "test.h"

class TestParser {
public:
  TestParser();
  bool loadFromFile(QString filepath);
  QList<Test> tests();

protected:
  QJsonDocument doc;
};

LineData readLineFromObject(QJsonObject lineObj);

#endif // TESTPARSER_H
