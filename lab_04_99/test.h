#ifndef TEST_H
#define TEST_H

#include "shapeutils.h"
#include <QDebug>
#include <QList>
#include <QString>

struct EllipseSpectre {
  EllipseData ellipseData;
  qreal step;
  qsizetype count;
};

struct CircleSpectre {
  CircleData circleData;
  qreal step;
  qsizetype count;
};

class Test {
public:
  Test();
  Test(QString name, QString desc, QList<CircleData> circles,
       QList<EllipseData> ellipses, QList<EllipseSpectre> ellipseSpectres,
       QList<CircleSpectre> circleSpectres, QColor backgroundColor);
  QString name() const;
  QString desc() const;
  QColor backgroundColor() const;
  QList<CircleData> circles() const;
  QList<EllipseData> ellipses() const;
  QList<EllipseSpectre> ellipseSpectres() const;
  QList<CircleSpectre> circleSpectres() const;

  bool isEmpty();

private:
  QString name_;
  QString desc_;
  QList<CircleData> circles_;
  QList<EllipseData> ellipses_;
  QList<EllipseSpectre> ellipseSpectres_;
  QList<CircleSpectre> circleSpectres_;
  QColor backgroundColor_;
};
QDebug operator<<(QDebug stream, const Test &test);

#endif // TEST_H
