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

      // backgroundColor
      QColor backgroundColor;
      if (testJson.contains("backgroundColor")) {
        backgroundColor = QColor(testJson["backgroundColor"].toString());
      } else {
        backgroundColor = QColor(Qt::white);
      }

      QList<CircleData> circles;
      QList<EllipseData> ellipses;
      QList<EllipseSpectre> ellipseSpectres;
      QList<CircleSpectre> circleSpectres;

      if (testJson.contains("circles")) {
        QJsonArray circlesJson = testJson["circles"].toArray();
        for (const auto &circleJson : circlesJson) {
          if (circleJson.isObject()) {
            auto circleObj = circleJson.toObject();
            circles.append(parseCircleData(circleObj));
          }
        }
      }
      if (testJson.contains("ellipses")) {
        QJsonArray ellipsesJson = testJson["ellipses"].toArray();
        for (const auto &ellipseJson : ellipsesJson) {
          if (ellipseJson.isObject()) {
            auto ellipseObj = ellipseJson.toObject();
            ellipses.append(parseEllipseData(ellipseObj));
          }
        }
      }
      if (testJson.contains("ellipseSpectres")) {
        QJsonArray ellipseSpectresJson = testJson["ellipseSpectres"].toArray();
        for (const auto &ellipseSpectreJson : ellipseSpectresJson) {
          if (ellipseSpectreJson.isObject()) {
            auto ellipseSpectreObj = ellipseSpectreJson.toObject();
            EllipseSpectre ellipseSpectre;
            ellipseSpectre.ellipseData = parseEllipseData(ellipseSpectreObj);
            ellipseSpectre.step = ellipseSpectreObj["step"].toDouble();
            ellipseSpectre.count = ellipseSpectreObj["count"].toInt();
            ellipseSpectres.append(ellipseSpectre);
          }
        }
      }
      if (testJson.contains("circleSpectres")) {
        QJsonArray circleSpectresJson = testJson["circleSpectres"].toArray();
        for (const auto &circleSpectreJson : circleSpectresJson) {
          auto circleSpectreObj = circleSpectreJson.toObject();
          CircleSpectre circleSpectre;
          circleSpectre.circleData = parseCircleData(circleSpectreObj);
          circleSpectre.step = circleSpectreObj["step"].toDouble();
          circleSpectre.count = circleSpectreObj["count"].toInt();
          circleSpectres.append(circleSpectre);
        }
      }
      tests << Test(name, desc, circles, ellipses, ellipseSpectres,
                    circleSpectres, backgroundColor);
    }
  }
  return tests;
}

EllipseData TestParser::parseEllipseData(QJsonObject &json) {
  EllipseData ellipseData;
  ellipseData.a = json["a"].toDouble();
  ellipseData.b = json["b"].toDouble();
  ellipseData.color = parseColor(json);
  ellipseData.center = parseCenter(json);
  ellipseData.algorithm = parseAlgorithm(json);
  return ellipseData;
}

CircleData TestParser::parseCircleData(QJsonObject &json) {
  CircleData circleData;
  circleData.radius = json["r"].toDouble();
  circleData.color = parseColor(json);
  circleData.center = parseCenter(json);
  circleData.algorithm = parseAlgorithm(json);
  return circleData;
}

QPoint TestParser::parseCenter(QJsonObject &json) {
  QPoint center(0, 0);
  if (json.contains("center")) {
    auto centerJson = json["center"].toObject();
    center.setX(centerJson["x"].toInt());
    center.setY(centerJson["y"].toInt());
  }

  return center;
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

DrawingAlgorithm TestParser::parseAlgorithm(QJsonObject &json) {
  DrawingAlgorithm algorithm;
  auto algorithmStr = json["algorithm"].toString();
  if (algorithmStr == "Bresenham") {
    algorithm = DrawingAlgorithm::BRESENHAM;
  } else if (algorithmStr == "MiddlePoint") {
    algorithm = DrawingAlgorithm::MIDDLE_POINT;
  } else if (algorithmStr == "Canonical") {
    algorithm = DrawingAlgorithm::CANONICAL;
  } else if (algorithmStr == "Parametric") {
    algorithm = DrawingAlgorithm::PARAMETRIC;
  } else {
    algorithm = DrawingAlgorithm::BRESENHAM;
  }
  return algorithm;
}
