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

      // polygons
      QList<QPolygon> polygons;
      QJsonArray polygonsJson = testJson["polygons"].toArray();
      for (const auto &polygonJson : polygonsJson) {
        if (polygonJson.isArray()) {
          QJsonArray pointsJson = polygonJson.toArray();
          QPolygon polygon;
          for (const auto &pointJson : pointsJson) {
            if (pointJson.isObject()) {
              QJsonObject pointObj = pointJson.toObject();
              polygon << parsePoint(pointObj);
            }
          }
          polygons << polygon;
        }
      }

      QList<QPoint> shapePoints;
      QJsonArray clipShapeJson = testJson["cutter"].toArray();
      for (const auto &pointJson : clipShapeJson) {
        if (pointJson.isObject()) {
          QJsonObject pointObj = pointJson.toObject();
          shapePoints << parsePoint(pointObj);
        }
      }

      tests << Test(name, desc, polygons, shapePoints);
    }
  }
  return tests;
}

QPoint TestParser::parsePoint(QJsonObject &json) {
  return QPoint(json["x"].toInt(), json["y"].toInt());
}
