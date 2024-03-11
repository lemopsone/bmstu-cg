#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include "testparser.h"

void createNextWindow(MainWindow *w, const QList<Test>& tests, qsizetype &cur)
{
    if (cur < tests.size())
    {
        Test test = tests[cur++];
        qDebug() << test;
        w = new MainWindow(test);
        QObject::connect(w, &MainWindow::destroyed, [&]()
        {
            createNextWindow(w, tests, cur);
        });
        w->show();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestParser parser;
    parser.loadFromFile("./tests.json");
    QList<Test> tests = parser.tests();

    MainWindow *w = nullptr;
    qsizetype cur = 0;

    createNextWindow(w, tests, cur);
    return a.exec();
}


