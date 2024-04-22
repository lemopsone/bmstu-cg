#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

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

    // Если есть активный тест, провести тестирование
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
    // QRectF planeCoordinates = QRectF(-12, -8, 24, 16);

    this->scene = new CoordinateScene(graphicsWindowCoordinates,
                                      graphicsWindowCoordinates,
                                      this->ui->showCoordinatesRadio->isChecked());
    this->ui->graphicsView->setScene(this->scene);
    this->ui->graphicsView->scale(1, -1);
    this->ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
    this->ui->graphicsView->setMouseTracking(true);
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
    this->connect(this->ui->buttonDDA,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onDDAButtonClicked()));
    this->connect(this->ui->buttonBresenhamFloat,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onBresenhamFloatButtonClicked()));
    this->connect(this->ui->buttonBresenhamInt,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onBresenhamIntButtonClicked()));
    this->connect(this->ui->buttonBresenhamSmooth,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onBresenhamSmoothButtonClicked()));
    this->connect(this->ui->buttonWu,
                  SIGNAL(clicked()),
                  this,
                  SLOT(onWuButtonClicked()));
    this->setAttribute(Qt::WA_DeleteOnClose, true);
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

void MainWindow::onDDAButtonClicked()
{
    QLine line = readLine();
    QColor color = Qt::red;
    scene->addLine(line, LineType::DDA, color);
}

void MainWindow::onBresenhamFloatButtonClicked()
{
    QLine line = readLine();
    QColor color = Qt::blue;
    scene->addLine(line, LineType::BRESENHAM_FLOAT, color);
}

void MainWindow::onBresenhamIntButtonClicked()
{
    QLine line = readLine();
    QColor color = Qt::green;
    scene->addLine(line, LineType::BRESENHAM_INT, color);
}

void MainWindow::onBresenhamSmoothButtonClicked()
{
    QLine line = readLine();
    QColor color = Qt::black;
    scene->addLine(line, LineType::BRESENHAM_SMOOTH, color);
}

void MainWindow::onWuButtonClicked()
{
    QLine line = readLine();
    QColor color = Qt::yellow;
    scene->addLine(line, LineType::WU, color);
}

QLine MainWindow::readLine()
{
    QPoint p1, p2;
    p1.setX(ui->firstPointEditX->text().toInt());
    p1.setY(ui->firstPointEditY->text().toInt());
    p1.setX(ui->secondPointEditX->text().toInt());
    p1.setY(ui->secondPointEditY->text().toInt());
    return QLine(p1, p2);
}

void MainWindow::populateTestData(Test test)
{

}
