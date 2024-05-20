#ifndef POINTTABLEVIEWMODEL_H
#define POINTTABLEVIEWMODEL_H

#include <QAbstractListModel>

#include "pointrecord.h"

class PointTableViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit PointTableViewModel(QObject *parent = nullptr);
    ~PointTableViewModel();

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void appendRecord(PointRecord record);
    void deleteRow(ssize_t index);

    void removeAllRecords();

private:
    QList<PointRecord> *values;
};

#endif // POINTTABLEVIEWMODEL_H
