#include "linerecord.h"
#include <qdebug.h>

LineRecord::LineRecord(QLine line) { this->setLine(line); }

QLine LineRecord::getLine(void) const { return this->line; }

void LineRecord::setLine(const QLine line) {
  this->line = line;
  this->setText(this->serializeLine(line));
}

int LineRecord::getId(void) const { return this->id; }

void LineRecord::setId(const int id) { this->id = id; }

QString LineRecord::getText(void) const { return this->text; }

void LineRecord::setText(const QString text) { this->text = text; }

QPoint LineRecord::getP1(void) const { return this->line.p1(); }

void LineRecord::setP1(const QPoint p1) { this->line.setP1(p1); }

QPoint LineRecord::getP2(void) const { return this->line.p2(); }

void LineRecord::setP2(const QPoint p2) { this->line.setP2(p2); }

QString LineRecord::serializePoint(QPoint point) {
  return QString("(%1, %2)").arg(point.x()).arg(point.y());
}

QString LineRecord::serializeLine(QLine line) {
  return QString("%1 ; %2")
      .arg(this->serializePoint(line.p1()))
      .arg(this->serializePoint(line.p2()));
}
