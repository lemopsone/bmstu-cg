#ifndef TEST_H
#define TEST_H

#include <QDebug>
#include <QList>
#include <QString>

#include "linedrawer.h"

struct SpectreData {
  LineData lineData;
  qreal angle;
};

class Test {
public:
  Test();
  Test(QString name, QString desc, QList<LineData> lines,
       QList<SpectreData> spectres, QColor backgroundColor = Qt::white);
  QString name() const;
  QString desc() const;
  QList<LineData> lines() const;
  QList<SpectreData> spectres() const;
  QColor backgroundColor() const;

  bool isEmpty();

private:
  QString name_;
  QString desc_;
  QList<LineData> lines_;
  QList<SpectreData> spectres_;
  QColor backgroundColor_;
};
QDebug operator<<(QDebug stream, const Test &test);

#endif // TEST_H
