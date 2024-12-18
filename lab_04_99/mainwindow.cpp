#include "mainwindow.h"

#include "shapeutils.h"
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
  this->connect(this->ui->colorChangeButton, SIGNAL(clicked()),
                SLOT(onColorChangeButtonClicked()));
  this->connect(this->ui->bgColorChangeButton, SIGNAL(clicked()),
                SLOT(onBgColorChangeButtonClicked()));
  this->connect(this->ui->clearScreenButton, SIGNAL(clicked()), this,
                SLOT(onClearScreenButtonClicked()));
  this->connect(this->ui->drawCircleButton, SIGNAL(clicked()), this,
                SLOT(onDrawCircleButtonClicked()));
  this->connect(this->ui->drawEllipseButton, SIGNAL(clicked()), this,
                SLOT(onDrawEllipseButtonClicked()));
  this->connect(this->ui->drawEllipseSpectreButton, SIGNAL(clicked()), this,
                SLOT(onDrawEllipseSpectreButtonClicked()));
  this->connect(this->ui->drawCircleSpectreButton, SIGNAL(clicked()), this,
                SLOT(onDrawCircleSpectreButtonClicked()));

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

void MainWindow::onDrawCircleButtonClicked() {
  CircleData data;
  data.center = this->readCenter();
  data.radius = ui->circleRadiusEdit->text().toInt();
  data.color = this->lineColor;
  data.algorithm = readAlgorithm();

  this->scene->addCircle(data);
}

void MainWindow::onDrawEllipseButtonClicked() {
  EllipseData data;
  data.center = this->readCenter();
  data.a = ui->ellipseWidthEdit->text().toInt();
  data.b = ui->ellipseHeightEdit->text().toInt();
  data.color = this->lineColor;
  data.algorithm = readAlgorithm();

  this->scene->addEllipse(data);
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

void MainWindow::onDrawEllipseSpectreButtonClicked() {
  EllipseData data;
  data.center = this->readCenter();
  data.a = ui->ellipseWidthEdit->text().toInt();
  data.b = ui->ellipseHeightEdit->text().toInt();
  data.color = this->lineColor;
  data.algorithm = readAlgorithm();

  qreal step = ui->spectreStepEdit->text().toDouble();
  qsizetype count = ui->spectreCountEdit->text().toInt();

  this->scene->addEllipseSpectre(data, step, count);
}

void MainWindow::onDrawCircleSpectreButtonClicked() {
  CircleData data;
  data.center = this->readCenter();
  data.radius = ui->circleRadiusEdit->text().toInt();
  data.color = this->lineColor;
  data.algorithm = readAlgorithm();

  qreal step = ui->spectreStepEdit->text().toDouble();
  qsizetype count = ui->spectreCountEdit->text().toInt();

  this->scene->addCircleSpectre(data, step, count);
}

QPoint MainWindow::readCenter() {
  QPoint center;
  center.setX(ui->centerEditX->text().toInt());
  center.setY(ui->centerEditY->text().toInt());

  return center;
}

void MainWindow::populateTestData(Test test) {
  scene->setGridVisibility(false);
  scene->setColor(test.backgroundColor());

  for (const auto &circle : test.circles()) {
    scene->addCircle(circle);
  }
  for (const auto &ellipse : test.ellipses()) {
    scene->addEllipse(ellipse);
  }
  for (const auto &circleSpectre : test.circleSpectres()) {
    scene->addCircleSpectre(circleSpectre.circleData, circleSpectre.step,
                            circleSpectre.count);
  }
  for (const auto &ellipseSpectre : test.ellipseSpectres()) {
    scene->addEllipseSpectre(ellipseSpectre.ellipseData, ellipseSpectre.step,
                             ellipseSpectre.count);
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

DrawingAlgorithm MainWindow::readAlgorithm() {
  if (ui->radioCanonical->isChecked()) {
    return DrawingAlgorithm::CANONICAL;
  } else if (ui->radioParametric->isChecked()) {
    return DrawingAlgorithm::PARAMETRIC;
  } else if (ui->radioBresenham->isChecked()) {
    return DrawingAlgorithm::BRESENHAM;
  } else {
    return DrawingAlgorithm::MIDDLE_POINT;
  }
}
