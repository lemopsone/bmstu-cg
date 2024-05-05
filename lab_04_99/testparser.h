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

private:
  EllipseData parseEllipseData(QJsonObject &json);
  CircleData parseCircleData(QJsonObject &json);
  QPoint parseCenter(QJsonObject &json);
  QColor parseColor(QJsonObject &json);
  DrawingAlgorithm parseAlgorithm(QJsonObject &json);
};

#endif // TESTPARSER_H
