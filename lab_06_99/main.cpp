#include <QApplication>
#include <QDebug>
#include <QDir>

#include "mainwindow.h"
#include "testparser.h"

void createNextWindow(MainWindow *w, const QList<Test> &tests, qsizetype &cur) {
  if (cur < tests.size()) {
    if (cur == 0) {
      qDebug() << "***** НАЧАЛО ФУНКЦИОНАЛЬНОГО ТЕСТИРОВАНИЯ *****";
    }
    Test test = tests[cur++];
    qDebug() << test;
    w = new MainWindow(test);
    QObject::connect(w, &MainWindow::destroyed,
                     [&]() { createNextWindow(w, tests, cur); });
  } else if (tests.empty()) {
    w = new MainWindow(Test());
    w->show();
  } else {
    qDebug() << "***** КОНЕЦ ФУНКЦИОНАЛЬНОГО ТЕСТИРОВАНИЯ *****";
    qDebug() << "Тестов пройдено:" << tests.count()
             << "| результаты находятся в папке ./func/";
    QCoreApplication::quit();
  }
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  TestParser parser;
  QList<Test> tests;
  if (argc > 1) {
    QString appPath = QCoreApplication::applicationDirPath() + "/" + argv[1];
    parser.loadFromFile(appPath);
    tests = parser.tests();
  }

  MainWindow *w = nullptr;
  qsizetype cur = 0;

  createNextWindow(w, tests, cur);

  return a.exec();
}
