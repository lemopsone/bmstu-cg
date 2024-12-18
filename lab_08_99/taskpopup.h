#ifndef TASKPOPUP_H
#define TASKPOPUP_H

#include <QDialog>

namespace Ui {
class TaskPopUp;
}

class TaskPopUp : public QDialog {
  Q_OBJECT

public:
  explicit TaskPopUp(QWidget *parent = nullptr);
  ~TaskPopUp();

public slots:
  void onCloseButtonClicked();

private:
  Ui::TaskPopUp *ui;
};

#endif // TASKPOPUP_H
