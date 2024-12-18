#include "test.h"

Test::Test() {}

Test::Test(QString name, QString desc, QList<QLine> lines, QList<QPoint> clipShape)
    : name_(name), desc_(desc), lines_(lines), shapePoints_(clipShape) {}

QString Test::name() const { return this->name_; }

QString Test::desc() const { return this->desc_; }

QList<QLine> Test::lines() const { return this->lines_; }

QList<QPoint> Test::shapePoints() const { return this->shapePoints_; }

bool Test::isEmpty() { return this->name_.isEmpty() && this->desc_.isEmpty(); }

QDebug operator<<(QDebug stream, const Test &test) {
  stream << "Текущий тест: " << test.name() << Qt::endl;
  stream << "Описание: " << test.desc() << Qt::endl;

  return stream;
}
