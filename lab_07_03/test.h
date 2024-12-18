#ifndef TEST_H
#define TEST_H

#include <QColor>
#include <QDebug>
#include <QList>
#include <QString>
#include <QPoint>
#include <QLine>

class Test {
public:
  Test();
  Test(QString name, QString desc, QList<QLine> lines, QList<QPoint> clipShape);
  QString name() const;
  QString desc() const;
  QColor color() const;
  QList<QLine> lines() const;
  QList<QPoint> shapePoints() const;

  bool isEmpty();

private:
  QString name_;
  QString desc_;
  QList<QLine> lines_;
  QList<QPoint> shapePoints_;
};
QDebug operator<<(QDebug stream, const Test &test);

#endif // TEST_H
