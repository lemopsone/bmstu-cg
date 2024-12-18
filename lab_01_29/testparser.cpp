#include "testparser.h"

TestParser::TestParser() {}

bool TestParser::loadFromFile(QString filepath)
{
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

QList<Test> TestParser::tests()
{
    QList<Test> tests;

    QJsonArray data = this->doc.array();
    for (const auto& jsonValue : data) {
        if (jsonValue.isObject()) {
            QJsonObject testJson = jsonValue.toObject();
            // name
            QString name = testJson["name"].toString();

            // rectangle
            QJsonArray rectangle = testJson["rectangle"].toArray();
            QPointF rectPoints[4];
            for (qsizetype i = 0; i < 4; i++) {
                QJsonArray point = rectangle[i].toArray();
                double x = point.at(0).toDouble();
                double y = point.at(1).toDouble();
                rectPoints[i] = QPointF(x, y);
            }
            MyRectF rect(rectPoints);

            // points
            QList<QPointF> points;
            QJsonArray pointsArray = testJson["points"].toArray();
            for (const auto& pointData : pointsArray) {
                QJsonArray point = pointData.toArray();
                double x = point.at(0).toDouble();
                double y = point.at(1).toDouble();
                points.append(QPointF(x, y));
            }

            tests.append(Test(name, points, rect));
        }
    }

    return tests;
}
