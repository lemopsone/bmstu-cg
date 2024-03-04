#include "mainwindow.h"
#include "scenepoint.h"
#include "ui_mainwindow.h"
#include "mytrianglef.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);


    // Сборка всплывающего окна с заданием
    this->initTaskPopUp();


    // Сборка сцены
    this->initGraphicsScene();

    // Сборка View/Model для таблицы точек на плоскости
    this->initPointTableViewModel();

    // Сигналы, слоты
    this->connectAll();

    MyTriangleF triangle = MyTriangleF(
        QPointF(0, 0),
        QPointF(7, 3),
        QPointF(10, 10)
    );
    triangle.bisectIntersection();
    triangle.bisectIntersection2();
}

MainWindow::~MainWindow()
{
    delete this->scene;
    delete this->ui;
    delete this->pointTableModel;
    delete this->addRecordDialog;
}

void MainWindow::initTaskPopUp()
{
    this->taskPop = new TaskPopUp();
    this->ui->showCoordinatesRadio->setChecked(true);
}

void MainWindow::initGraphicsScene()
{
    QRectF viewGeometry = this->ui->graphicsView->geometry();
    QRectF graphicsWindowCoordinates = QRectF(-viewGeometry.width() / 2, -viewGeometry.height() / 2, viewGeometry.width(), viewGeometry.height());
    QRectF planeCoordinates = QRectF(-5, -5, 10, 10);

    this->scene = new CoordinateScene(graphicsWindowCoordinates, planeCoordinates, this->ui->showCoordinatesRadio->isChecked());
    this->ui->graphicsView->setScene(this->scene);
    this->ui->graphicsView->scale(1, -1);
    this->ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
    this->ui->graphicsView->setMouseTracking(true);
}

void MainWindow::initPointTableViewModel()
{
    this->pointTableModel = new PointTableViewModel();
    this->ui->pointsTableView->setModel(this->pointTableModel);
    this->ui->pointsTableView->setColumnWidth(0, this->ui->pointsTableView->width());
    this->ui->pointsTableView->setColumnWidth(1, this->ui->pointsTableView->width());
    this->ui->pointsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Инциализация диалога для добавления точки
    this->initAddRecordDialog();
}

void MainWindow::initAddRecordDialog()
{
    this->addRecordDialog = new AddRecordDialog();
}

void MainWindow::connectAll()
{
    this->connect(this->ui->showTaskButton, SIGNAL(clicked()),
                  this, SLOT(showTaskPopUp()));
    this->connect(this->ui->showCoordinatesRadio, SIGNAL(toggled(bool)),
                  this, SLOT(onShowCoordinatesRadioToggle(bool)));
    this->connect(this->ui->sizeInput, SIGNAL(editingFinished()),
                  this, SLOT(onSizeInputEditFinished()));
    this->connect(this->scene, SIGNAL(zoomChanged(double)),
                  this, SLOT(onSceneZoomChanged(double)));
    this->connect(this->scene, SIGNAL(sceneClicked(QPointF)),
                  this, SLOT(onSceneClicked(QPointF)));
    this->connect(this->scene, SIGNAL(mousePositionChanged(QPointF)),
                  this, SLOT(onSceneMousePositionChanged(QPointF)));
    this->connect(this->scene, SIGNAL(pointDeleted(ssize_t)),
                  this, SLOT(onScenePointDeleted(ssize_t)));
    this->connect(this->ui->addRecordButton, SIGNAL(clicked()),
                  this, SLOT(onAddRecordButtonClicked()));
    this->connect(this->ui->clearRecordsButton, SIGNAL(clicked()),
                  this, SLOT(onClearAllRecordsClicked()));
    this->connect(this->addRecordDialog, SIGNAL(validPointAdded(QPointF)),
                  this, SLOT(onTablePointAdded(QPointF)));
    this->connect(this->ui->deleteRecordButton, SIGNAL(clicked()),
                  this, SLOT(onDeleteButtonClicked()));
    this->connect(this->ui->pointsTableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                  this, SLOT(onSelectionChanged(QItemSelection,QItemSelection)));
}

void MainWindow::showTaskPopUp()
{
    this->taskPop->show();
}

void MainWindow::onAddRecordButtonClicked()
{
    this->addRecordDialog->show();
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

void MainWindow::onSceneClicked(QPointF point)
{
    point.setX(std::round(point.x() * 1000.0) / 1000.0);
    point.setY(std::round(point.y() * 1000.0) / 1000.0);
    this->scene->addPoint(new ScenePoint(point, this->scene));
    this->scene->update();
    this->pointTableModel->appendRecord(PointRecord(point));
}

void MainWindow::onSceneMousePositionChanged(QPointF currentPos)
{
    this->ui->cursorPositionX->setText(QString::number(std::round(currentPos.x() * 1000.0) / 1000.0));
    this->ui->cursorPositionY->setText(QString::number(std::round(currentPos.y() * 1000.0) / 1000.0));
}

void MainWindow::onScenePointDeleted(ssize_t pointIndex)
{
    this->pointTableModel->deleteRow(pointIndex);
}

void MainWindow::onClearAllRecordsClicked()
{
    this->pointTableModel->removeAllRecords();
    this->scene->removeAllPoints();
}

void MainWindow::onTablePointAdded(QPointF point)
{
    point.setX(std::round(point.x() * 1000.0) / 1000.0);
    point.setY(std::round(point.y() * 1000.0) / 1000.0);
    this->scene->addItem(new ScenePoint(point, this->scene));
    this->scene->update();
    this->pointTableModel->appendRecord(PointRecord(point));
}

void MainWindow::onSelectionChanged(const QItemSelection &selected, const QItemSelection &unselected)
{
    for (auto index : selected.indexes())
    {
        if (index.row() < this->scene->items().count())
        {
            this->scene->setPointSelection(index.row(), true);
        }
    }
    for (auto index : unselected.indexes())
    {
        if (index.row() < this->scene->items().count())
        {
            this->scene->setPointSelection(index.row(), false);
        }
    }
    this->scene->update();
}

void MainWindow::onDeleteButtonClicked()
{
    QList points = this->scene->selectedItems();
    for (auto point : points)
    {
        if (this->scene->items().contains(point))
        {
            this->scene->removePoint(point);
        }
    }

    /*
     * QModelIndexList хранит в себе индексы в том порядке, в котором записи в таблице были выделены
     * таким образом, если выделить сначала 3 запись, потом 1, потом 2, получим QModelIndexList({3}, {2}, {1})
     * чтобы нормализовать входные данные, необходимо отсортировать QList по убыванию
     */
    QModelIndexList selectedRowsIndexes = this->ui->pointsTableView->selectionModel()->selectedRows();
    std::sort(selectedRowsIndexes.rbegin(), selectedRowsIndexes.rend());
    for (auto index : selectedRowsIndexes)
    {
        if (this->pointTableModel->rowCount() > index.row())
        {
            this->pointTableModel->deleteRow(index.row());
        }
    }

    this->ui->pointsTableView->selectionModel()->clearSelection();
}

