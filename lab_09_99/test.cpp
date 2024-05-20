#include "test.h"

Test::Test() {}

Test::Test(QString name, QString desc, QList<QPolygon> polygons, QPolygon shape)
    : name_(name), desc_(desc), polygons_(polygons), shape_(shape) {}

QString Test::name() const { return this->name_; }

QString Test::desc() const { return this->desc_; }

QPolygon Test::shape() const { return this->shape_; }

QList<QPolygon> Test::polygons() const { return this->polygons_; }

bool Test::isEmpty() { return this->name_.isEmpty() && this->desc_.isEmpty(); }

QDebug operator<<(QDebug stream, const Test &test) {
  stream << "Текущий тест: " << test.name() << Qt::endl;
  stream << "Описание: " << test.desc() << Qt::endl;

  return stream;
}
