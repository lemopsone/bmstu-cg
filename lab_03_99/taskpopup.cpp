#include "taskpopup.h"

#include "ui_taskpopup.h"

TaskPopUp::TaskPopUp(QWidget *parent) : QDialog(parent), ui(new Ui::TaskPopUp) {
  ui->setupUi(this);
  connect(ui->closeTaskButton, SIGNAL(clicked()), this,
          SLOT(onCloseButtonClicked()));
}

TaskPopUp::~TaskPopUp() { delete ui; }

void TaskPopUp::onCloseButtonClicked() { this->close(); }
