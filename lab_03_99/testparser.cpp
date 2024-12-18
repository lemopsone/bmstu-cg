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

      QColor backgroundColor;
      if (testJson.contains("backgroundColor")) {
        backgroundColor = QColor(testJson["backgroundColor"].toString());
      } else {
        backgroundColor = Qt::white;
      }

      QList<LineData> lines;
      QList<SpectreData> spectres;

      // lines
      QJsonArray lineArr = testJson["lines"].toArray();
      for (const auto &line : lineArr) {
        auto lineObj = line.toObject();
        lines << readLineFromObject(lineObj);
      }

      // spectres
      QJsonArray spectreArr = testJson["spectres"].toArray();
      for (const auto &spectre : spectreArr) {
        SpectreData spectreData;
        auto spectreObj = spectre.toObject();
        spectreData.lineData = readLineFromObject(spectreObj);
        spectreData.angle = spectreObj["angle"].toDouble();
        spectres << spectreData;
      }

      tests.append(Test(name, desc, lines, spectres, backgroundColor));
    }
  }

  return tests;
}

LineData readLineFromObject(QJsonObject lineObj) {
  LineData data;
  qreal x1 = lineObj["x1"].toDouble();
  qreal x2 = lineObj["x2"].toDouble();
  qreal y1 = lineObj["y1"].toDouble();
  qreal y2 = lineObj["y2"].toDouble();
  data.line = QLine(x1, y1, x2, y2);
  QString type = lineObj["type"].toString();
  if (type == "DDA")
    data.type = LineType::DDA;
  else if (type == "BresenhamInt")
    data.type = LineType::BRESENHAM_INT;
  else if (type == "BresenhamFloat")
    data.type = LineType::BRESENHAM_FLOAT;
  else if (type == "BresenhamSmooth")
    data.type = LineType::BRESENHAM_SMOOTH;
  else if (type == "Wu")
    data.type = LineType::WU;

  if (!lineObj["color"].isUndefined()) {
    data.color = QColor(lineObj["color"].toString());
  }

  return data;
}
