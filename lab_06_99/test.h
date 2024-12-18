#ifndef TEST_H
#define TEST_H

#include <QColor>
#include <QDebug>
#include <QList>
#include <QString>
#include <QPoint>

class Test {
public:
  Test();
  Test(QString name, QString desc, QList<QPoint> points, QPoint seed,
       QColor color = Qt::black);
  QString name() const;
  QString desc() const;
  QColor color() const;
  QList<QPoint> points() const;
  QPoint seed() const;

  bool isEmpty();

private:
  QString name_;
  QString desc_;
  QList<QPoint> points_;
  QPoint seed_;
  QColor color_;
};
QDebug operator<<(QDebug stream, const Test &test);

#endif // TEST_H
