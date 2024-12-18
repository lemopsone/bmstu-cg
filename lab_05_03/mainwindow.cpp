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

  this->ui->colorChangeButton->setPalette(QPalette(Qt::black));

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
  this->connect(this->ui->colorChangeButton, SIGNAL(clicked()),
                SLOT(onColorChangeButtonClicked()));
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

  this->setAttribute(Qt::WA_DeleteOnClose, true);
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
  this->scene->setDelay(delay);
  if (this->scene->isLocked()) {
    qint64 time = this->scene->createFilledPolygon();
    // popup with elapsed time
    QMessageBox::information(this, "Замер времени",
                             QString("Время выполнения: %1 мс").arg(time));
  }
  ui->closeFigureButton->setEnabled(false);
}

void MainWindow::onColorChangeButtonClicked() {
  this->lineColor = QColorDialog::getColor();
  this->ui->colorChangeButton->setPalette(QPalette(this->lineColor));
  this->scene->setColor(lineColor);
  this->scene->update();
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

void MainWindow::populateTestData(Test test) {
  for (auto point : test.points()) {
    this->scene->addPolygonPoint(point);
  }
  this->scene->setLocked(true);
  this->scene->setColor(test.color());
  this->scene->createFilledPolygon();

  QString filename = QCoreApplication::applicationDirPath() + "/" +
                     QString("../func/%1.png").arg(test.name());
  this->scene->getPolygonImage().save(filename, "png");

  this->deleteLater();
}
