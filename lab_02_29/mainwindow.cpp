#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Test test, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);

    // Сборка всплывающего окна с заданием
    this->initTaskPopUp();

    // Сборка сцены
    this->initGraphicsScene();

    // Сигналы, слоты
    this->connectAll();

    // Если есть активный тест, добавить все точки и построить прямоугольник
    if (!test.isEmpty()) {
        this->populateTestData(test);
    }
}

MainWindow::~MainWindow()
{
    delete this->scene;
    delete this->ui;
    delete this->taskPop;
}

void MainWindow::initTaskPopUp()
{
    this->taskPop = new TaskPopUp();
    this->ui->showCoordinatesRadio->setChecked(true);
}

void MainWindow::initGraphicsScene()
{
    QRectF viewGeometry = this->ui->graphicsView->geometry();
    QRectF graphicsWindowCoordinates = QRectF(-viewGeometry.width() / 2,
                                              -viewGeometry.height() / 2,
                                              viewGeometry.width(),
                                              viewGeometry.height());
    QRectF planeCoordinates = QRectF(-12, -8, 24, 16);

    this->scene = new CoordinateScene(graphicsWindowCoordinates,
                                      planeCoordinates,
                                      this->ui->showCoordinatesRadio->isChecked());
    this->ui->graphicsView->setScene(this->scene);
    this->ui->graphicsView->scale(1, -1);
    this->ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
    this->ui->graphicsView->setMouseTracking(true);

    this->updateUndoRedoStates();
}

void MainWindow::connectAll()
{
    this->connect(this->ui->showTaskButton, SIGNAL(clicked()), this, SLOT(showTaskPopUp()));
    this->connect(this->ui->showCoordinatesRadio,
                  SIGNAL(toggled(bool)),
                  this,
                  SLOT(onShowCoordinatesRadioToggle(bool)));

    this->connect(this->ui->sizeInput,
                  SIGNAL(editingFinished()),
                  this,
                  SLOT(onSizeInputEditFinished()));

    this->connect(this->scene, SIGNAL(zoomChanged(double)), this, SLOT(onSceneZoomChanged(double)));

    this->connect(this->scene,
                  SIGNAL(mousePositionChanged(QPointF)),
                  this,
                  SLOT(onSceneMousePositionChanged(QPointF)));

    this->connect(this->ui->drawHouseButton,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onDrawHouseButtonClicked()));
    this->connect(this->ui->moveButton,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onMoveButtonClicked()));
    this->connect(this->ui->rotateButton,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onRotateButtonClicked()));
    this->connect(this->ui->scaleButton,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onScaleButtonClicked()));
    this->connect(this->ui->undoButton,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onUndoButtonClicked()));
    this->connect(this->ui->redoButton,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onRedoButtonClicked()));
    this->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::showTaskPopUp()
{
    this->taskPop->show();
}

void MainWindow::onShowCoordinatesRadioToggle(bool value)
{
    this->scene->setGridVisibility(value);
}

void MainWindow::onSizeInputEditFinished()
{
    this->scene->zoomAtPercent(this->ui->sizeInput->text().toDouble());
}

void MainWindow::onSceneZoomChanged(double value)
{
    this->ui->sizeInput->setText(QString::number(std::round(value * 1000.0) / 1000.0));
}

void MainWindow::onSceneMousePositionChanged(QPointF currentPos)
{
    this->ui->cursorPositionX->setText(
        QString::number(std::round(currentPos.x() * 1000.0) / 1000.0));
    this->ui->cursorPositionY->setText(
        QString::number(std::round(currentPos.y() * 1000.0) / 1000.0));
}

void MainWindow::onAutoScaleRadioToggle(bool state)
{
    this->scene->setAutoScale(state);
}

void MainWindow::onMoveButtonClicked()
{
    bool xOk, yOk;
    Move move {
        .x = this->ui->moveXLineEdit->text().toDouble(&xOk),
        .y = this->ui->moveYLineEdit->text().toDouble(&yOk)
    };
    if (!xOk || !yOk) {
        this->ui->moveXLineEdit->setText(QString("0.0"));
        this->ui->moveYLineEdit->setText(QString("0.0"));
    } else if (this->scene->house != nullptr) {
        this->scene->house->processAction(Action{ .type = MOVE, .move = move });
        this->scene->update();
    }
    this->updateUndoRedoStates();
}

void MainWindow::onRotateButtonClicked()
{
    bool angleOk;
    QPointF center;
    double angleDegrees = this->ui->rotateAngleLineEdit->text().toDouble(&angleOk);
    Rotate rotate{
        .angle = qDegreesToRadians(angleDegrees),
        .center = nullptr
    };
    if (!angleOk) {
        this->ui->rotateAngleLineEdit->setText(QString("0.0"));
    } else if (this->scene->house != nullptr) {
        if (this->ui->radioCustomPoint->isChecked() && centerIsValid()) {
            center = readCenter();
            rotate.center = &center;
        }
        this->scene->house->processAction(Action{ .type = ROTATE, .rotate = rotate });
        this->scene->update();
    }
    this->updateUndoRedoStates();
}

void MainWindow::onScaleButtonClicked()
{
    bool xOk, yOk;
    QPointF center;
    Scale scale{
        .kx = this->ui->scaleXLineEdit->text().toDouble(&xOk),
        .ky = this->ui->scaleYLineEdit->text().toDouble(&yOk),
        .center = nullptr
    };
    if (!xOk || !yOk) {
        this->ui->scaleXLineEdit->setText(QString("1.0"));
        this->ui->scaleYLineEdit->setText(QString("1.0"));
    } else if (this->scene->house != nullptr) {
        if (this->ui->radioCustomPoint->isChecked() && centerIsValid()) {
            center = readCenter();
            scale.center = &center;
        }
        this->scene->house->processAction(Action{ .type = SCALE, .scale = scale });
        this->scene->update();
    }
    this->updateUndoRedoStates();
}

void MainWindow::onUndoButtonClicked()
{
    if (this->scene->house) {
        this->scene->house->undo();
        this->scene->update();
    }
    this->updateUndoRedoStates();
}

void MainWindow::onRedoButtonClicked()
{
    if (this->scene->house) {
        this->scene->house->redo();
        this->scene->update();
    }
    this->updateUndoRedoStates();
}

void MainWindow::populateTestData(Test test)
{
    return;
}

void MainWindow::onDrawHouseButtonClicked()
{
    if (this->scene->house == nullptr) {
        this->scene->createHouse();
        this->ui->drawHouseButton->setText(QString("Очистить"));
    } else {
        this->scene->removeHouse();
        this->ui->drawHouseButton->setText(QString("Нарисовать\nдом"));
    }
    this->updateUndoRedoStates();
}

bool MainWindow::centerIsValid()
{
    bool xOk, yOk;
    this->ui->centerXEdit->text().toDouble(&xOk);
    this->ui->centerYEdit->text().toDouble(&yOk);
    if (!xOk || !yOk) {
        this->ui->centerXEdit->setText(QString("0.0"));
        this->ui->centerYEdit->setText(QString("0.0"));
    }
    return xOk && yOk;
}

QPointF MainWindow::readCenter()
{
    return QPointF(
        this->ui->centerXEdit->text().toDouble(),
        this->ui->centerYEdit->text().toDouble());
}

void MainWindow::updateUndoRedoStates()
{
    auto undoButton = this->ui->undoButton;
    auto redoButton = this->ui->redoButton;

    if (this->scene->house == nullptr) {
        undoButton->setEnabled(false);
        redoButton->setEnabled(false);
    } else {
        undoButton->setEnabled(!this->scene->house->undo_stack.empty());
        redoButton->setEnabled(!this->scene->house->redo_stack.empty());
    }
}
