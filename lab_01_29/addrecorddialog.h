#ifndef ADDRECORDDIALOG_H
#define ADDRECORDDIALOG_H

#include <QDialog>

namespace Ui {
class AddRecordDialog;
}

class AddRecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRecordDialog(QWidget *parent = nullptr);
    ~AddRecordDialog();

signals:
    void validPointAdded(QPointF);

protected slots:
    void onCancelClicked(void);
    void onAddClicked(void);

private:
    Ui::AddRecordDialog *ui;
};

#endif // ADDRECORDDIALOG_H
