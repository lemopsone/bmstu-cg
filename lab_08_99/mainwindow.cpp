#include "mainwindow.h"
#include <QMessageBox>

#include "linetableviewmodel.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Test test, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  this->ui->setupUi(this);

  // Сборка всплывающего окна с заданием
  this->initTaskPopUp();

  // Сборка сцены
  this->initGraphicsScene();

  // Инициализация таблицы
  this->initLineTable();
  this->initShapePointTable();

  // Сигналы, слоты
  this->connectAll();

  this->ui->bgColorChangeButton->setPalette(QPalette(Qt::white));
  this->ui->lineColorChangeButton->setPalette(QPalette(Qt::black));
  this->ui->clipColorChangeButton->setPalette(QPalette(Qt::red));
  this->ui->clipShapeColorChangeButton->setPalette(QPalette(Qt::blue));

  // Если есть активный тест, провести тестирование
  if (!test.isEmpty()) {
    this->populateTestData(test);
  }
}

MainWindow::~MainWindow() {
  delete this->scene;
  delete this->ui;
  delete this->taskPop;
  delete this->tableModel;
  delete this->shapePointModel;
}

void MainWindow::initTaskPopUp() { this->taskPop = new TaskPopUp(); }

void MainWindow::initGraphicsScene() {
  this->scene = new CoordinateScene(QRectF(0, 0, 1024, 800), SceneColors());
  this->ui->graphicsView->setScene(this->scene);
  this->ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
  this->ui->graphicsView->setMouseTracking(true);
}

void MainWindow::initLineTable() {
  this->tableModel = new LineTableViewModel(this);
  this->ui->linesTableView->setModel(this->tableModel);
  this->ui->linesTableView->setColumnWidth(0,
                                           this->ui->linesTableView->width());
  this->ui->linesTableView->setColumnWidth(1,
                                           this->ui->linesTableView->width());
  this->ui->linesTableView->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
}

void MainWindow::initShapePointTable() {
  this->shapePointModel = new PointTableViewModel(this);
  this->ui->clipShapePointsTableView->setModel(this->shapePointModel);
  this->ui->clipShapePointsTableView->setColumnWidth(
      0, this->ui->clipShapePointsTableView->width());
  this->ui->clipShapePointsTableView->setColumnWidth(
      1, this->ui->clipShapePointsTableView->width());
  this->ui->clipShapePointsTableView->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
}

void MainWindow::connectAll() {
  this->connect(this->ui->showTaskButton, SIGNAL(clicked()), this,
                SLOT(showTaskPopUp()));
  this->connect(this->scene, SIGNAL(mousePositionChanged(QPointF)), this,
                SLOT(onSceneMousePositionChanged(QPointF)));
  this->connect(this->scene, SIGNAL(lineCreated(QPoint, QPoint)), this,
                SLOT(onLineCreated(QPoint, QPoint)));
  this->connect(this->ui->clearScreenButton, SIGNAL(clicked()), this,
                SLOT(onClearScreenButtonClicked()));
  this->connect(this->ui->addLineButton, SIGNAL(clicked()), this,
                SLOT(onAddLineButtonClicked()));
  this->connect(this->ui->drawModeLineButton, SIGNAL(toggled(bool)), this,
                SLOT(onDrawModeToggle(bool)));
  this->connect(this->ui->clipButton, SIGNAL(clicked()), this,
                SLOT(onClipButtonClicked()));
  this->connect(this->ui->bgColorChangeButton, SIGNAL(clicked()), this,
                SLOT(onBgColorChangeButtonClicked()));
  this->connect(this->ui->lineColorChangeButton, SIGNAL(clicked()), this,
                SLOT(onLineColorChangeButtonClicked()));
  this->connect(this->ui->clipColorChangeButton, SIGNAL(clicked()), this,
                SLOT(onClipColorChangeButtonClicked()));
  this->connect(this->ui->clipShapeColorChangeButton, SIGNAL(clicked()), this,
                SLOT(onClipShapeColorChangeButtonClicked()));
  this->connect(this->scene, SIGNAL(clipShapePointAdded(QPoint)), this,
                SLOT(onClipShapePointAdded(QPoint)));
  this->connect(this->scene, SIGNAL(clipShapeLockToggled(bool)), this,
                SLOT(onClipShapeLockToggled(bool)));
  this->connect(this->scene, SIGNAL(clipShapeCleared()), this,
                SLOT(onClipShapeCleared()));
  this->connect(this->ui->lockButton, SIGNAL(clicked()), this,
                SLOT(onLockClipShapeButtonClicked()));


  this->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::showTaskPopUp() { this->taskPop->show(); }

void MainWindow::onLockClipShapeButtonClicked() { this->scene->lockClipShape(); }

void MainWindow::onClipShapeLockToggled(bool locked) {
  this->ui->lockButton->setEnabled(!locked);
  this->ui->clipButton->setEnabled(locked);
}


void MainWindow::onClipShapeCleared() {
  this->shapePointModel->removeAllRecords();
  this->ui->lockButton->setEnabled(false);
  this->ui->clipButton->setEnabled(false);
}

void MainWindow::onClipShapePointAdded(QPoint point) {
  this->shapePointModel->appendRecord(PointRecord(point));
  this->ui->lockButton->setEnabled(this->shapePointModel->rowCount() > 2);
}

void MainWindow::onSceneMousePositionChanged(QPointF point) {
  this->ui->cursorPositionX->setText(QString::number(std::round(point.x())));
  this->ui->cursorPositionY->setText(QString::number(std::round(point.y())));
}

void MainWindow::onLineCreated(QPoint start, QPoint end) {
  this->tableModel->appendRecord(LineRecord(QLine(start, end)));
}

void MainWindow::onDrawModeToggle(bool checked) {
  if (checked) {
    this->scene->setDrawMode(DrawMode::LINE);
  } else {
    this->scene->setDrawMode(DrawMode::CLIPPER);
  }
}

void MainWindow::onClipButtonClicked() { this->scene->clipLines(); }

void MainWindow::onBgColorChangeButtonClicked() {
  this->colors.bgColor = QColorDialog::getColor();
  this->scene->setBackgroundColor(this->colors.bgColor);
  this->ui->bgColorChangeButton->setPalette(QPalette(this->colors.bgColor));
}

void MainWindow::onLineColorChangeButtonClicked() {
  this->colors.lineColor = QColorDialog::getColor();
  this->scene->setLineColor(this->colors.lineColor);
  this->ui->lineColorChangeButton->setPalette(QPalette(this->colors.lineColor));
}

void MainWindow::onClipColorChangeButtonClicked() {
  this->colors.clipColor = QColorDialog::getColor();
  this->scene->setClipColor(this->colors.clipColor);
  this->ui->clipColorChangeButton->setPalette(QPalette(this->colors.clipColor));
}

void MainWindow::onClipShapeColorChangeButtonClicked() {
  this->colors.clipShapeColor = QColorDialog::getColor();
  this->scene->setClipShapeColor(this->colors.clipShapeColor);
  this->ui->clipShapeColorChangeButton->setPalette(
      QPalette(this->colors.clipShapeColor));
}

void MainWindow::onClearScreenButtonClicked() {
  this->tableModel->removeAllRecords();
  this->shapePointModel->removeAllRecords();
  this->ui->lockButton->setEnabled(false);
  this->ui->clipButton->setEnabled(false);
  scene->clearScene();
  scene->update();
}

void MainWindow::onAddLineButtonClicked() {
  QPoint p1 = readP1();
  QPoint p2 = readP2();
  this->tableModel->appendRecord(LineRecord(QLine(p1, p2)));
  this->scene->createLine(p1, p2);
}

QPoint MainWindow::readP1() {
  return QPoint(ui->lineP1X->text().toInt(), ui->lineP1Y->text().toInt());
}

QPoint MainWindow::readP2() {
  return QPoint(ui->lineP2X->text().toInt(), ui->lineP2Y->text().toInt());
}

void MainWindow::populateTestData(Test test) {
  for (const auto &line : test.lines()) {
    scene->createLine(line.p1(), line.p2());
  }
  scene->createClipShape(QPolygon(test.shapePoints()));

  scene->clipLines();

  QString filename = QCoreApplication::applicationDirPath() + "/" +
                     QString("../func/%1.png").arg(test.name());
  this->scene->getImage()->save(filename, "png");

  this->deleteLater();
}
