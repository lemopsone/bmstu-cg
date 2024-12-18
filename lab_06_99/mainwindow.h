#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColorDialog>
#include <QItemSelection>
#include <QMainWindow>

#include "coordinatescene.h"
#include "fill.h"
#include "pointtableviewmodel.h"
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
  void initPointTable(void);

  void connectAll(void);
  void populateTestData(Test test);

public slots:
  void showTaskPopUp();
  void onFgColorChangeButtonClicked();
  void onEdgeColorChangeButtonClicked();
  void onSceneClicked(QPoint point);
  void onSceneMousePositionChanged(QPointF point);
  void onClearScreenButtonClicked();
  void onAddPointButtonClicked();
  void onLockFigureButtonClicked();
  void onFillFigureButtonClicked();
  void onSeedPointSet(QPoint seed);

private:
  Ui::MainWindow *ui;
  FillColors colors;
  PointTableViewModel *tableModel;

  QPoint readPoint();
  QPoint readSeed();
  int readDelay();
  FillColors readColors();
  QColor readForegroundColor();
  QColor readEdgeColor();
};

#endif // MAINWINDOW_H
