#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>

#include "addrecorddialog.h"
#include "coordinatescene.h"
#include "pointtableviewmodel.h"
#include "taskpopup.h"
#include "test.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Test test, QWidget *parent = nullptr);
    ~MainWindow();
    TaskPopUp *taskPop;
    CoordinateScene *scene;
    AddRecordDialog *addRecordDialog;

    void initTaskPopUp(void);
    void initGraphicsScene(void);
    void initPointTableViewModel(void);
    void initAddRecordDialog(void);

    void connectAll(void);

public slots:
    void showTaskPopUp();
    void onShowCoordinatesRadioToggle(bool checked);
    void onSizeInputEditFinished();
    void onSceneZoomChanged(double value);
    void onSceneMousePositionChanged(QPointF currentPos);
    void onSceneClicked(QPointF point);
    void onScenePointDeleted(ssize_t pointIndex);
    void onAddRecordButtonClicked();
    void onClearAllRecordsClicked();
    void onTablePointAdded(QPointF point);
    void onSelectionChanged(const QItemSelection &, const QItemSelection &);
    void onDeleteButtonClicked();
    void onRectangleAcceptButtonClicked();
    void onRectangleResetButtonClicked();
    void onSolveTaskButtonClicked();
    void onAutoScaleRadioToggle(bool);
    void onSceneChanged();

private:
    Ui::MainWindow *ui;
    PointTableViewModel *pointTableModel;
    void addSolution(double minAngle, QLineF line, MyTriangleF triangle);
    void clearSolution(void);
    void populateTestData(Test);
    qreal findSolution(const MyRectF &rect,
                       const QList<ScenePoint *> &points,
                       QLineF &resultLine,
                       MyTriangleF &resultTriangle);
};

#endif // MAINWINDOW_H
