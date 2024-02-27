#include "addrecorddialog.h"
#include "ui_addrecorddialog.h"

AddRecordDialog::AddRecordDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddRecordDialog)
{
    ui->setupUi(this);

    this->connect(this->ui->cancelButton, SIGNAL(clicked()),
            this, SLOT(onCancelClicked()));
    this->connect(this->ui->addRecordButton, SIGNAL(clicked()),
            this, SLOT(onAddClicked()));
}

AddRecordDialog::~AddRecordDialog()
{
    delete ui;
}

void AddRecordDialog::onCancelClicked()
{
    this->close();
}

void AddRecordDialog::onAddClicked()
{
    qreal x, y;
    bool validX, validY;
    x = this->ui->inputX->text().toDouble(&validX);
    y = this->ui->inputY->text().toDouble(&validY);

    if (!validX || !validY)
    {
        this->ui->exceptionLabel->setText(QString("Некорректные координаты"));
    }
    else
    {
        QPointF point = QPointF(x, y);
        emit validPointAdded(point);
        this->ui->exceptionLabel->setText(QString());
        this->close();
    }
}
