#include "mainwindow.h"
#include <QFile>

#include "ui_mainwindow.h"

MainWindow::MainWindow(Test test, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  this->ui->setupUi(this);

  // Сборка всплывающего окна с заданием
  this->initTaskPopUp();

  // Сборка сцены
  this->initGraphicsScene();

  // Сигналы, слоты
  this->connectAll();

  this->ui->colorChangeButton->setPalette(QPalette(Qt::black));
  this->ui->bgColorChangeButton->setPalette(QPalette(Qt::white));

  // Если есть активный тест, провести тестирование
  if (!test.isEmpty()) {
    this->populateTestData(test);
  }
}

MainWindow::~MainWindow() {
  delete this->scene;
  delete this->ui;
  delete this->taskPop;
  if (tc)
    delete tc;
}

void MainWindow::initTaskPopUp() {
  this->taskPop = new TaskPopUp();
  this->ui->showCoordinatesRadio->setChecked(true);
}

void MainWindow::initGraphicsScene() {
  QRectF viewGeometry = this->ui->graphicsView->geometry();
  QRectF graphicsWindowCoordinates =
      QRectF(-viewGeometry.width() / 2, -viewGeometry.height() / 2,
             viewGeometry.width(), viewGeometry.height());

  this->scene =
      new CoordinateScene(graphicsWindowCoordinates, graphicsWindowCoordinates,
                          this->ui->showCoordinatesRadio->isChecked());
  this->ui->graphicsView->setScene(this->scene);
  this->ui->graphicsView->scale(1, -1);
  this->ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
  this->ui->graphicsView->setMouseTracking(true);
}

void MainWindow::connectAll() {
  this->connect(this->ui->showTaskButton, SIGNAL(clicked()), this,
                SLOT(showTaskPopUp()));
  this->connect(this->ui->showCoordinatesRadio, SIGNAL(toggled(bool)), this,
                SLOT(onShowCoordinatesRadioToggle(bool)));

  this->connect(this->ui->sizeInput, SIGNAL(editingFinished()), this,
                SLOT(onSizeInputEditFinished()));

  this->connect(this->scene, SIGNAL(zoomChanged(double)), this,
                SLOT(onSceneZoomChanged(double)));

  this->connect(this->scene, SIGNAL(mousePositionChanged(QPointF)), this,
                SLOT(onSceneMousePositionChanged(QPointF)));
  this->connect(this->ui->drawLineButton, SIGNAL(clicked()), this,
                SLOT(onDrawLineButtonClicked()));
  this->connect(this->ui->drawSpectreButton, SIGNAL(clicked()), this,
                SLOT(onDrawSpectreButtonClicked()));
  this->connect(this->ui->colorChangeButton, SIGNAL(clicked()),
                SLOT(onColorChangeButtonClicked()));
  this->connect(this->ui->bgColorChangeButton, SIGNAL(clicked()),
                SLOT(onBgColorChangeButtonClicked()));
  this->connect(this->ui->clearScreenButton, SIGNAL(clicked()), this,
                SLOT(onClearScreenButtonClicked()));
  this->connect(this->ui->timeComparisonButton, SIGNAL(clicked()), this,
                SLOT(onTimeComparisonButtonClicked()));
  this->connect(this->ui->graphButton, SIGNAL(clicked()), this,
                SLOT(onGraphButtonClicked()));
  this->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::showTaskPopUp() { this->taskPop->show(); }

void MainWindow::onShowCoordinatesRadioToggle(bool value) {
  this->scene->setGridVisibility(value);
}

void MainWindow::onSizeInputEditFinished() {
  this->scene->zoomAtPercent(this->ui->sizeInput->text().toDouble());
}

void MainWindow::onSceneZoomChanged(double value) {
  this->ui->sizeInput->setText(
      QString::number(std::round(value * 1000.0) / 1000.0));
}

void MainWindow::onSceneMousePositionChanged(QPointF currentPos) {
  this->ui->cursorPositionX->setText(
      QString::number(std::round(currentPos.x() * 1000.0) / 1000.0));
  this->ui->cursorPositionY->setText(
      QString::number(std::round(currentPos.y() * 1000.0) / 1000.0));
}

void MainWindow::onAutoScaleRadioToggle(bool state) {
  this->scene->setAutoScale(state);
}

void MainWindow::onDrawLineButtonClicked() {
  LineData data = getLineData();
  scene->addLine(data);
}

void MainWindow::onDrawSpectreButtonClicked() {
  LineData data = getLineData();
  qreal angle = ui->spectreLineEdit->text().toDouble();
  scene->addSpectre(data, angle);
}

LineData MainWindow::getLineData() {
  LineData ret;
  ret.line = readLine();
  if (ui->radioDDA->isChecked())
    ret.type = LineType::DDA;
  else if (ui->radioBresenhamFloat->isChecked())
    ret.type = LineType::BRESENHAM_FLOAT;
  else if (ui->radioBresenhamInt->isChecked())
    ret.type = LineType::BRESENHAM_INT;
  else if (ui->radioBresenhamSmooth->isChecked())
    ret.type = LineType::BRESENHAM_SMOOTH;
  else if (ui->radioWu->isChecked())
    ret.type = LineType::WU;
  ret.color = lineColor;

  return ret;
}

void MainWindow::onColorChangeButtonClicked() {
  this->lineColor = QColorDialog::getColor();
  this->ui->colorChangeButton->setPalette(QPalette(this->lineColor));
}

void MainWindow::onBgColorChangeButtonClicked() {
  QColor color = QColorDialog::getColor();
  this->scene->setColor(color);
  this->ui->bgColorChangeButton->setPalette(QPalette(color));
}

void MainWindow::onClearScreenButtonClicked() {
  scene->clear();
  scene->update();
}

void MainWindow::onTimeComparisonButtonClicked() {
  this->tc = new TimeComparison();
  tc->show();
}

void MainWindow::onGraphButtonClicked() {
  this->graph = new StepLengthGraph;
  graph->show();
}

QLine MainWindow::readLine() {
  QPoint p1, p2;
  p1.setX(ui->firstPointEditX->text().toInt());
  p1.setY(ui->firstPointEditY->text().toInt());
  p2.setX(ui->secondPointEditX->text().toInt());
  p2.setY(ui->secondPointEditY->text().toInt());
  return QLine(p1, p2);
}

void MainWindow::populateTestData(Test test) {
  scene->setGridVisibility(false);
  scene->setColor(test.backgroundColor());
  for (auto line : test.lines()) {
    scene->addLine(line);
  }
  for (auto spectre : test.spectres()) {
    scene->addSpectre(spectre.lineData, spectre.angle);
  }

  QImage image(scene->getGraphicsWindow().size().toSize(),
               QImage::Format_ARGB32);

  QPainter painter(&image);
  scene->render(&painter);
  image.mirror();
  QString filename = QCoreApplication::applicationDirPath() + "/" +
                     QString("../func/%1.png").arg(test.name());
  image.save(filename, "png");

  this->deleteLater();
}
