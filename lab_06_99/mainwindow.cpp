#include "mainwindow.h"
#include <QMessageBox>

#include "ui_mainwindow.h"

MainWindow::MainWindow(Test test, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  this->ui->setupUi(this);

  // Сборка всплывающего окна с заданием
  this->initTaskPopUp();

  // Сборка сцены
  this->initGraphicsScene();

  // Инициализация таблицы
  this->initPointTable();

  // Сигналы, слоты
  this->connectAll();

  this->ui->edgeColorChangeButton->setPalette(QPalette(Qt::black));
  this->ui->fgColorChangeButton->setPalette(QPalette(Qt::blue));

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
}

void MainWindow::initTaskPopUp() { this->taskPop = new TaskPopUp(); }

void MainWindow::initGraphicsScene() {
  this->scene = new CoordinateScene(QRectF(0, 0, 1024, 800));
  this->ui->graphicsView->setScene(this->scene);
  this->ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
  this->ui->graphicsView->setMouseTracking(true);
}

void MainWindow::initPointTable() {
  this->tableModel = new PointTableViewModel(this);
  this->ui->pointsTableView->setModel(this->tableModel);
  this->ui->pointsTableView->setColumnWidth(0,
                                            this->ui->pointsTableView->width());
  this->ui->pointsTableView->setColumnWidth(1,
                                            this->ui->pointsTableView->width());
  this->ui->pointsTableView->horizontalHeader()->setSectionResizeMode(
      QHeaderView::Stretch);
}

void MainWindow::connectAll() {
  this->connect(this->ui->showTaskButton, SIGNAL(clicked()), this,
                SLOT(showTaskPopUp()));
  this->connect(this->scene, SIGNAL(mousePositionChanged(QPointF)), this,
                SLOT(onSceneMousePositionChanged(QPointF)));
  this->connect(this->scene, SIGNAL(sceneClicked(QPoint)), this,
                SLOT(onSceneClicked(QPoint)));
  this->connect(this->ui->clearScreenButton, SIGNAL(clicked()), this,
                SLOT(onClearScreenButtonClicked()));
  this->connect(this->ui->addPointButton, SIGNAL(clicked()), this,
                SLOT(onAddPointButtonClicked()));
  this->connect(this->ui->closeFigureButton, SIGNAL(clicked()), this,
                SLOT(onLockFigureButtonClicked()));
  this->connect(this->ui->fillFigureButton, SIGNAL(clicked()), this,
                SLOT(onFillFigureButtonClicked()));
  this->connect(this->ui->edgeColorChangeButton, SIGNAL(clicked()), this,
                SLOT(onEdgeColorChangeButtonClicked()));
  this->connect(this->ui->fgColorChangeButton, SIGNAL(clicked()), this,
                SLOT(onFgColorChangeButtonClicked()));
  this->connect(this->scene, SIGNAL(seedPointSet(QPoint)), this,
                SLOT(onSeedPointSet(QPoint)));

  this->setAttribute(Qt::WA_DeleteOnClose, true);
}

void MainWindow::onSeedPointSet(QPoint seed) {
  this->ui->seedX->setText(QString::number(seed.x()));
  this->ui->seedY->setText(QString::number(seed.y()));
}

void MainWindow::showTaskPopUp() { this->taskPop->show(); }

void MainWindow::onSceneMousePositionChanged(QPointF point) {
  this->ui->cursorPositionX->setText(QString::number(std::round(point.x())));
  this->ui->cursorPositionY->setText(QString::number(std::round(point.y())));
}

void MainWindow::onSceneClicked(QPoint point) {
  this->ui->newPointEditX->setText(QString::number(point.x()));
  this->ui->newPointEditY->setText(QString::number(point.y()));
  this->tableModel->appendRecord(PointRecord(point));
  ui->closeFigureButton->setEnabled(tableModel->rowCount() > 2);
}

int MainWindow::readDelay() { return ui->delayLineEdit->text().toInt(); }

void MainWindow::onFillFigureButtonClicked() {
  int delay = readDelay();
  QPoint seed = readSeed();
  if (this->scene->isLocked()) {
    long time = fill(this->scene, this->colors, seed, delay);
    // popup with elapsed time
    if (delay == 0) {
      QMessageBox::information(this, "Замер времени",
                               QString("Время выполнения: %1 мс").arg(time));
    }
  }
  ui->closeFigureButton->setEnabled(false);
}

void MainWindow::onEdgeColorChangeButtonClicked() {
  this->colors.edge = QColorDialog::getColor();
  this->ui->edgeColorChangeButton->setPalette(QPalette(this->colors.edge));
  this->scene->setEdgeColor(this->colors.edge);
  this->scene->update();
}

void MainWindow::onFgColorChangeButtonClicked() {
  this->colors.foreground = QColorDialog::getColor();
  this->ui->fgColorChangeButton->setPalette(QPalette(this->colors.foreground));
}

void MainWindow::onClearScreenButtonClicked() {
  this->tableModel->removeAllRecords();
  scene->clearScene();
  scene->update();
  ui->closeFigureButton->setEnabled(true);
}

void MainWindow::onAddPointButtonClicked() {
  QPoint point = readPoint();
  this->tableModel->appendRecord(PointRecord(point));
  this->scene->addPolygonPoint(point);
}

void MainWindow::onLockFigureButtonClicked() { this->scene->setLocked(true); }

QPoint MainWindow::readPoint() {
  QPoint center;
  center.setX(ui->newPointEditX->text().toInt());
  center.setY(ui->newPointEditY->text().toInt());

  return center;
}

QPoint MainWindow::readSeed() {
  QPoint seed;
  seed.setX(ui->seedX->text().toInt());
  seed.setY(ui->seedY->text().toInt());

  return seed;
}

void MainWindow::populateTestData(Test test) {
  for (auto point : test.points()) {
    this->scene->addPolygonPoint(point);
  }
  this->scene->setLocked(true);

  FillColors colors = { .foreground = test.color() };
  fill(scene, colors, test.seed(), 0);

  QString filename = QCoreApplication::applicationDirPath() + "/" +
                     QString("../func/%1.png").arg(test.name());
  this->scene->getImage()->save(filename, "png");

  this->deleteLater();
}
