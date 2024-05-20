#ifndef LINERECORD_H
#define LINERECORD_H

#include <QLine>
#include <QPoint>
#include <QString>

class LineRecord {
public:
  LineRecord();
  LineRecord(QLine line);

  QLine getLine(void) const;
  void setLine(const QLine line);

  int getId(void) const;
  void setId(const int id);

  QString getText(void) const;
  void setText(const QString text);

  QPoint getP1(void) const;
  void setP1(const QPoint p1);

  QPoint getP2(void) const;
  void setP2(const QPoint p2);

  QString serializePoint(QPoint point);
  QString serializeLine(QLine line);

private:
  QLine line;
  int id;
  QString text;
  QString icon;
};

#endif // LINERECORD_H
