#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QItemSelection>
#include <QMainWindow>

#include "coordinatescene.h"
#include "taskpopup.h"
#include "test.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(Test test, QWidget *parent = nullptr);
  ~MainWindow();
  TaskPopUp *taskPop;
  CoordinateScene *scene;

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
  void onColorChangeButtonClicked();
  void onBgColorChangeButtonClicked();
  void onClearScreenButtonClicked();
  void onDrawCircleButtonClicked();
  void onDrawEllipseButtonClicked();
  void onDrawEllipseSpectreButtonClicked();
  void onDrawCircleSpectreButtonClicked();

private:
  Ui::MainWindow *ui;
  QColor lineColor;
  QPoint readCenter();
  DrawingAlgorithm readAlgorithm();
};

#endif // MAINWINDOW_H