#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QItemSelection>
#include <QMainWindow>

#include "coordinatescene.h"
#include "pointtableviewmodel.h"
#include "linetableviewmodel.h"
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
  void initLineTable(void);
  void initShapePointTable(void);

  void connectAll(void);
  void populateTestData(Test test);

public slots:
  void showTaskPopUp();
  void onBgColorChangeButtonClicked();
  void onLineColorChangeButtonClicked();
  void onClipColorChangeButtonClicked();
  void onClipShapeColorChangeButtonClicked();
  void onSceneMousePositionChanged(QPointF point);
  void onClearScreenButtonClicked();
  void onAddLineButtonClicked();
  void onDrawModeToggle(bool checked);
  void onClipButtonClicked();
  void onLineCreated(QPoint start, QPoint end);
  void onClipShapePointAdded(QPoint point);
  void onClipShapeLockToggled(bool locked);
  void onClipShapeCleared();
  void onLockClipShapeButtonClicked();

private:
  Ui::MainWindow *ui;
  SceneColors colors;
  LineTableViewModel *tableModel;
  PointTableViewModel *shapePointModel;

  QPoint readP1();
  QPoint readP2();
};

#endif // MAINWINDOW_H
