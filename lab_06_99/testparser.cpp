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

      // color
      QColor color = parseColor(testJson);

      // points
      QList<QPoint> points;
      QJsonArray pointsJson = testJson["vertexes"].toArray();
      for (const auto &pointJson : pointsJson) {
        if (pointJson.isObject()) {
          QJsonObject pointObj = pointJson.toObject();
          points << parsePoint(pointObj);
        }
      }

      // seed
      QJsonObject seedObj = testJson["seed"].toObject();
      QPoint seed = parsePoint(seedObj);

      tests << Test(name, desc, points, seed, color);
    }
  }
  return tests;
}

QPoint TestParser::parsePoint(QJsonObject &json) {
  return QPoint(json["x"].toInt(), json["y"].toInt());
}

QColor TestParser::parseColor(QJsonObject &json) {
  QColor color;
  if (json.contains("color")) {
    color = QColor(json["color"].toString());
  } else {
    color = QColor(Qt::black);
  }
  return color;
}
