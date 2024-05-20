#ifndef LINETABLEVIEWMODEL_H
#define LINETABLEVIEWMODEL_H

#include <QAbstractListModel>

#include "linerecord.h"

class LineTableViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LineTableViewModel(QObject *parent = nullptr);
    ~LineTableViewModel();

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void appendRecord(LineRecord record);
    void deleteRow(ssize_t index);

    void removeAllRecords();

private:
    QList<LineRecord> *values;
};

#endif // LINETABLEVIEWMODEL_H
