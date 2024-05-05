#include "test.h"

Test::Test() {}

Test::Test(QString name, QString desc, QList<CircleData> circles,
           QList<EllipseData> ellipses, QList<EllipseSpectre> ellipseSpectres,
           QList<CircleSpectre> circleSpectres, QColor backgroundColor)
    : name_(name), desc_(desc), circles_(circles), ellipses_(ellipses),
      ellipseSpectres_(ellipseSpectres), circleSpectres_(circleSpectres),
      backgroundColor_(backgroundColor) {}

QString Test::name() const { return this->name_; }

QString Test::desc() const { return this->desc_; }

bool Test::isEmpty() { return this->name_.isEmpty() && this->desc_.isEmpty(); }

QList<CircleData> Test::circles() const { return this->circles_; }

QList<EllipseData> Test::ellipses() const { return this->ellipses_; }

QList<EllipseSpectre> Test::ellipseSpectres() const {
  return this->ellipseSpectres_;
}

QList<CircleSpectre> Test::circleSpectres() const {
  return this->circleSpectres_;
}

QColor Test::backgroundColor() const { return this->backgroundColor_; }

QDebug operator<<(QDebug stream, const Test &test) {
  stream << "Текущий тест: " << test.name() << Qt::endl;
  stream << "Описание: " << test.desc();

  return stream;
}
