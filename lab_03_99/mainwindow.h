#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <QColorDialog>

#include "coordinatescene.h"
#include "taskpopup.h"
#include "test.h"
#include "timecomparison.h"
#include "steplengthgraph.h"

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
    TimeComparison *tc;
    StepLengthGraph *graph;

    void initTaskPopUp(void);
    void initGraphicsScene(void);

    void connectAll(void);
    void populateTestData(Test test);

public slots:
    void showTaskPopUp();
    void onShowCoordinatesRadioToggle(bool checked);
    void onSizeInputEditFinished();
    void onSceneZoomChanged(double value);
    void onSceneMousePositionChanged(QPointF currentPos);
    void onAutoScaleRadioToggle(bool);
    void onDrawLineButtonClicked();
    void onDrawSpectreButtonClicked();
    void onColorChangeButtonClicked();
    void onBgColorChangeButtonClicked();
    void onClearScreenButtonClicked();
    void onTimeComparisonButtonClicked();
    void onGraphButtonClicked();

private:
    Ui::MainWindow *ui;
    LineData getLineData();
    QLine readLine();
    QColor lineColor;
};

#endif // MAINWINDOW_H
