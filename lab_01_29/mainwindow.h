#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>

#include "taskpopup.h"
#include "coordinatescene.h"
#include "pointtableviewmodel.h"
#include "addrecorddialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
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
    void onSelectionChanged(const QItemSelection&, const QItemSelection&);
    void onDeleteButtonClicked();

private:
    Ui::MainWindow *ui;
    PointTableViewModel *pointTableModel;
};

#endif // MAINWINDOW_H
