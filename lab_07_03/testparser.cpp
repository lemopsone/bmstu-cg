#include "testparser.h"

TestParser::TestParser() {}

bool TestParser::loadFromFile(QString filepath) {
  QFile file(filepath);
  if (!file.open(QIODevice::ReadOnly)) {
    qDebug() << "Failed to open the file:" << file.errorString();
    return false;
  }
  QByteArray jsonData = file.readAll();
  file.close();
  QJsonParseError error;
  QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);
  if (error.error != QJsonParseError::NoError) {
    qDebug() << "Failed to parse JSON:" << error.errorString();
    return false;
  }
  this->doc = jsonDoc;
  return true;
}

QList<Test> TestParser::tests() {
  QList<Test> tests;

  QJsonArray data = this->doc.array();
  for (const auto &jsonValue : data) {
    if (jsonValue.isObject()) {
      QJsonObject testJson = jsonValue.toObject();
      // name
      QString name = testJson["name"].toString();

      // desc
      QString desc = testJson["desc"].toString();

      // lines
      QList<QLine> lines;
      QJsonArray linesJson = testJson["lines"].toArray();
      for (const auto &lineJson : linesJson) {
        if (lineJson.isObject()) {
          QJsonObject lineObj = lineJson.toObject();
          lines << parseLine(lineObj);
        }
      }
      QJsonObject clipShapeJson = testJson["cutter"].toObject();
      QList<QPoint> clipShape = parseClipShape(clipShapeJson);

      tests << Test(name, desc, lines, clipShape);
    }
  }
  return tests;
}

QPoint TestParser::parsePoint(QJsonObject &json) {
  return QPoint(json["x"].toInt(), json["y"].toInt());
}

QList<QPoint> TestParser::parseClipShape(QJsonObject &json) {
  QList<QPoint> clipShape;
  auto p1Json = json["p1"].toObject();
  auto p2Json = json["p2"].toObject();
  QPoint start = parsePoint(p1Json);
  QPoint end = parsePoint(p2Json);

  clipShape << start << end;
  return clipShape;
}

QLine TestParser::parseLine(QJsonObject &json) {
  auto p1Json = json["p1"].toObject();
  auto p2Json = json["p2"].toObject();
  QPoint start = parsePoint(p1Json);
  QPoint end = parsePoint(p2Json);

  return QLine(start, end);
}

