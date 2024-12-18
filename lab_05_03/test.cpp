#include "test.h"

Test::Test() {}

Test::Test(QString name, QString desc, QList<QPoint> points, QColor color)
    : name_(name), desc_(desc), points_(points), color_(color) {}

QString Test::name() const { return this->name_; }

QString Test::desc() const { return this->desc_; }

QColor Test::color() const { return this->color_; }

QList<QPoint> Test::points() const { return this->points_; }

bool Test::isEmpty() { return this->name_.isEmpty() && this->desc_.isEmpty(); }

QDebug operator<<(QDebug stream, const Test &test) {
  stream << "Текущий тест: " << test.name() << Qt::endl;
  stream << "Описание: " << test.desc() << Qt::endl;

  return stream;
}
