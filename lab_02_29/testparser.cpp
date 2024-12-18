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

            // desc
            QString desc = testJson["desc"].toString();

            // actions
            QList<Action> actions;
            QJsonArray actionsJson = testJson["actions"].toArray();
            for (auto actionRef : actionsJson) {
                QJsonObject action = actionRef.toObject();
                QString actionType = action["type"].toString();
                if (actionType == "rotate") {
                    Rotate rotate = {
                        .angle = qDegreesToRadians(action["angle"].toDouble()),
                    };
                    if (!action["center"].isUndefined()) {
                        QJsonObject centerJson = action["center"].toObject();
                        double x = centerJson["x"].toDouble();
                        double y = centerJson["y"].toDouble();
                        rotate.center = QPointF(x, y);
                    }
                    actions.append(
                        Action{ .type = ROTATE, .rotate = rotate }
                        );
                } else if (actionType == "move") {
                    Move move = {
                        .x = action["x"].toDouble(),
                        .y = action["y"].toDouble()
                    };
                    actions.append(
                        Action{ .type = MOVE, .move = move }
                        );
                } else if (actionType == "scale") {
                    Scale scale = {
                        .kx = action["kx"].toDouble(),
                        .ky = action["ky"].toDouble()
                    };

                    if (!action["center"].isUndefined()) {
                        QJsonObject centerJson = action["center"].toObject();
                        double x = centerJson["x"].toDouble();
                        double y = centerJson["y"].toDouble();
                        scale.center = QPointF(x, y);
                    }
                    actions.append(
                        Action{ .type = SCALE, .scale = scale }
                        );
                }
            }

            tests.append(Test(name, desc, actions));
        }
    }

    return tests;
}
