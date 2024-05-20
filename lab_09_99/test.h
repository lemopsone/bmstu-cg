#ifndef TEST_H
#define TEST_H

#include <QColor>
#include <QDebug>
#include <QList>
#include <QString>
#include <QPoint>
#include <QPolygon>

class Test {
public:
  Test();
  Test(QString name, QString desc, QList<QPolygon> polygons, QPolygon shape);
  QString name() const;
  QString desc() const;

  QPolygon shape() const;
  QList<QPolygon> polygons() const;

  bool isEmpty();

private:
  QString name_;
  QString desc_;
  QList<QPolygon> polygons_;
  QPolygon shape_;
};
QDebug operator<<(QDebug stream, const Test &test);

#endif // TEST_H
