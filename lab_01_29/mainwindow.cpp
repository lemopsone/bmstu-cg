#include "mainwindow.h"
#include "scenepoint.h"
#include "scenetriangle.h"
#include "scenerectangle.h"
#include "sceneellipse.h"
#include "sceneline.h"
#include "ui_mainwindow.h"
#include "mytrianglef.h"

MainWindow::MainWindow(Test test, QWidget *parent)
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

    // Если есть активный тест, добавить все точки и построить прямоугольник
    if (!test.isEmpty())
    {
        this->populateTestData(test);
    }
}

MainWindow::~MainWindow()
{
    delete this->scene;
    delete this->ui;
    delete this->taskPop;
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
    QRectF planeCoordinates = QRectF(-6, -4.5, 12, 9);

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
    this->ui->pointsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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
    this->connect(this->ui->autoScaleRadio, SIGNAL(toggled(bool)),
                  this, SLOT(onAutoScaleRadioToggle(bool)));
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
    this->connect(this->ui->rectangleAcceptButton, SIGNAL(clicked()),
                this, SLOT(onRectangleAcceptButtonClicked()));
    this->connect(this->ui->rectangleResetButton, SIGNAL(clicked()),
                  this, SLOT(onRectangleResetButtonClicked()));
    this->connect(this->ui->solveTaskButton, SIGNAL(clicked()),
                  this, SLOT(onSolveTaskButtonClicked()));
    this->connect(this->scene, SIGNAL(itemsChanged()),
                  this, SLOT(onSceneChanged()));

    this->setAttribute(Qt::WA_DeleteOnClose);
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
    this->clearSolution();
}

void MainWindow::onSceneMousePositionChanged(QPointF currentPos)
{
    this->ui->cursorPositionX->setText(QString::number(std::round(currentPos.x() * 1000.0) / 1000.0));
    this->ui->cursorPositionY->setText(QString::number(std::round(currentPos.y() * 1000.0) / 1000.0));
}

void MainWindow::onScenePointDeleted(ssize_t pointIndex)
{
    this->pointTableModel->deleteRow(pointIndex);
    this->clearSolution();
}

void MainWindow::onClearAllRecordsClicked()
{
    this->pointTableModel->removeAllRecords();
    this->scene->removeAllPoints();
    this->clearSolution();
}

void MainWindow::onTablePointAdded(QPointF point)
{
    point.setX(std::round(point.x() * 1000.0) / 1000.0);
    point.setY(std::round(point.y() * 1000.0) / 1000.0);
    this->scene->addPoint(new ScenePoint(point, this->scene));
    this->clearSolution();
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
    for (auto point_ : points)
    {
        ScenePoint *point = qgraphicsitem_cast<ScenePoint *>(point_);
        if (point && this->scene->items().contains(point))
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
    this->clearSolution();
}


void MainWindow::onRectangleAcceptButtonClicked()
{
    QPointF points[4];
    for (qsizetype i = 0; i < this->ui->rectangleTableWidget->rowCount(); i++)
    {
        QTableWidgetItem *xptr = this->ui->rectangleTableWidget->item(i, 0);
        QTableWidgetItem *yptr = this->ui->rectangleTableWidget->item(i, 1);
        bool xValid = false, yValid = false;
        qreal x = xptr->data(Qt::DisplayRole).toDouble(&xValid);
        qreal y = yptr->data(Qt::DisplayRole).toDouble(&yValid);
        x = std::round(x * 1000.0) / 1000.0;
        y = std::round(y * 1000.0) / 1000.0;
        xptr->setData(Qt::DisplayRole, QString::number(x));
        yptr->setData(Qt::DisplayRole, QString::number(y));
        if (!xValid || !yValid)
        {
            this->ui->rectangleActionStatusLabel->setText(
                QString("Ошибка: некорректный формат входных данных")
            );
            return;
        }
        points[i] = QPointF(x, y);
    }
    MyRectF newRect = MyRectF(points);
    if (!newRect.isValid())
    {
        this->ui->rectangleActionStatusLabel->setText(
            QString("Ошибка: указанные точки не образуют прямоугольник")
        );
        return;
    }
    if (this->scene->getRectangles().size() != 0)
    {
        this->scene->removeRectangle((qsizetype)0);
    }
    this->ui->rectangleActionStatusLabel->setText(QString());
    this->scene->addRectangle(new SceneRectangle(newRect, this->scene));
    this->clearSolution();
}


void MainWindow::onRectangleResetButtonClicked()
{
    if (this->scene->getRectangles().size() != 0)
    {
        this->scene->removeRectangle((qsizetype)0);
    }
    for (qsizetype i = 0; i < this->ui->rectangleTableWidget->rowCount(); i++)
    {
        QTableWidgetItem *xptr = this->ui->rectangleTableWidget->item(i, 0);
        QTableWidgetItem *yptr = this->ui->rectangleTableWidget->item(i, 1);
        xptr->setData(Qt::DisplayRole, QString("0.000"));
        yptr->setData(Qt::DisplayRole, QString("0.000"));
    }
    this->clearSolution();
    this->ui->rectangleActionStatusLabel->setText(QString());
}

void MainWindow::onSolveTaskButtonClicked()
{
    QList<SceneRectangle *> rectangles = this->scene->getRectangles();
    QList<ScenePoint *> points = this->scene->getPoints();
    QLineF resultLine;
    MyTriangleF resultTriangle;
    if (rectangles.size() == 0 || points.size() < 3)
    {
        return;
    }
    MyRectF rectangle = rectangles[0]->rect;
    qreal minAngle = findSolution(rectangle, points, resultLine, resultTriangle);
    if (minAngle != 360)
    {
        this->addSolution(minAngle, resultLine, resultTriangle);
    }
}

void MainWindow::onAutoScaleRadioToggle(bool state)
{
    this->scene->setAutoScale(state);
}

void MainWindow::onSceneChanged()
{
    if (this->ui->autoScaleRadio->isChecked())
    {
        this->scene->autoScaleContents();
    }
}

void MainWindow::addSolution(double minAngle, QLineF line, MyTriangleF triangle)
{
    QPointF circleCenter = triangle.bisectIntersection();
    qreal radius = triangle.innerCircleRadius();

    this->scene->addLine(new SceneLine(line, this->scene));
    this->scene->addTriangle(new SceneTriangle(triangle, this->scene));
    this->scene->addEllipse(new SceneEllipse(circleCenter, radius, this->scene));

    QList<QPointF> trianglePoints = triangle.getPoints();
    QString solutionText = QString("Минимальный найденный угол - %1\n"
                                    "Треугольник с координатами (%2; %3), (%4; %5), (%6; %7)\n"
                                    "Центр вписанной окружности I - (%8; %9), радиус - %10\n"
                                    "Центр прямоугольника - (%11; %12)"
                                   ).arg(minAngle, 0, 'f', 3)
                                    .arg(trianglePoints[0].x()).arg(trianglePoints[0].y())
                                    .arg(trianglePoints[1].x()).arg(trianglePoints[1].y())
                                    .arg(trianglePoints[2].x()).arg(trianglePoints[2].y())
                                    .arg(circleCenter.x(), 0, 'f', 3).arg(circleCenter.y(), 0, 'f', 3)
                                    .arg(radius, 0, 'f', 3)
                                    .arg(line.p1().x()).arg(line.p1().y());
    this->ui->solutionTextEdit->setText(solutionText);
}

void MainWindow::clearSolution()
{
    QList<SceneLine *> lines = this->scene->getLines();
    QList<SceneEllipse *> ellipses = this->scene->getEllipses();
    QList<SceneTriangle *> triangles = this->scene->getTriangles();

    for (auto line : lines)
    {
        this->scene->removeLine(line);
    }
    for (auto ellipse : ellipses)
    {
        this->scene->removeEllipse(ellipse);
    }
    for (auto triangle : triangles)
    {
        this->scene->removeTriangle(triangle);
    }

    this->ui->solutionTextEdit->setText(QString());
    this->update();
}

void MainWindow::populateTestData(Test test)
{
    this->setWindowTitle(test.name());
    for (const auto& point : test.points())
    {
        this->onTablePointAdded(point);
    }

    QTableWidget *rectTable = this->ui->rectangleTableWidget;
    QList<QPointF> points = test.rect().getPoints();
    for (qsizetype i = 0; i < 4; i++)
    {
        QPointF point = points.at(i);
        QTableWidgetItem *xItem = rectTable->item(i, 0);
        QTableWidgetItem *yItem = rectTable->item(i, 1);
        xItem->setText(QString::number(point.x()));
        yItem->setText(QString::number(point.y()));
    }
    this->onRectangleAcceptButtonClicked();
    this->onSolveTaskButtonClicked();
}

qreal MainWindow::findSolution(const MyRectF &rect, const QList<ScenePoint *> &points, QLineF &resultLine, MyTriangleF &resultTriangle)
{
    qreal minAngle = 360;
    QPointF rectCenter = rect.center();

    for (qsizetype i = 0; i < points.size(); i++)
    {
        QPointF a = points[i]->coords;
        for (qsizetype j = i + 1; j < points.size(); j++)
        {
            QPointF b = points[j]->coords;
            for (qsizetype k = j + 1; k < points.size(); k++)
            {
                QPointF c = points[k]->coords;
                MyTriangleF triangle = MyTriangleF(a, b, c);
                if (!triangle.isValid())
                {
                    continue;
                }
                QPointF bisectIntersection = triangle.bisectIntersection();

                QLineF line = QLineF(rectCenter, bisectIntersection);
                qreal angle = std::abs(line.angle());
                angle = std::min(angle, 360 - angle);

                if (angle < minAngle)
                {
                    minAngle = angle;
                    resultLine = line;
                    resultTriangle = triangle;
                }
            }
        }
    }
    return minAngle;
}

