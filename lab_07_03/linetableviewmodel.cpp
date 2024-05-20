#include "linetableviewmodel.h"

LineTableViewModel::LineTableViewModel(QObject *parent)
    : QAbstractListModel(parent) {
  this->values = new QList<LineRecord>();
}

QVariant LineTableViewModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (orientation == Qt::Horizontal) {
    if (section == 0) {
      return QString("Точка 1");
    } else if (section == 1) {
      return QString("Точка 2");
    } else {
      return QVariant();
    }
  } else {
    return QString("%1").arg(section);
  }
}

int LineTableViewModel::rowCount(const QModelIndex &parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all other (valid) parents, rowCount() should return 0 so
  // that it does not become a tree model.
  if (parent.isValid())
    return 0;
  return values->count();
}

int LineTableViewModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 2;
}

QVariant LineTableViewModel::data(const QModelIndex &index, int role) const {
  QVariant value = QVariant();
  if (!index.isValid()) {
    return value;
  }

  if (role == Qt::DisplayRole) {
    // std::cout << "column no: " << index.column() << std::endl;
    if (index.column() == 0) {
      auto record = this->values->at(index.row());
      value = record.serializePoint(record.getP1());
    } else if (index.column() == 1) {
      auto record = this->values->at(index.row());
      value = record.serializePoint(record.getP2());
    }
  }
  return value;
}

void LineTableViewModel::appendRecord(LineRecord record) {
  this->beginInsertRows(QModelIndex(), this->values->size(),
                        this->values->size());
  this->values->push_back(record);
  this->endInsertRows();
}

void LineTableViewModel::deleteRow(ssize_t index) {
  this->beginRemoveRows(QModelIndex(), index, index);
  this->values->removeAt(index);
  this->endRemoveRows();
}

void LineTableViewModel::removeAllRecords() {
  this->beginRemoveRows(QModelIndex(), 0, this->values->size() - 1);
  this->values->clear();
  this->endRemoveRows();
}

LineTableViewModel::~LineTableViewModel() { delete this->values; }
