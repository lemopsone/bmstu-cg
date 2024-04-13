QT       += core gui

CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

common {
	message(adding common!)
	TARGET = lab_01_29
    SOURCES += \
        addrecorddialog.cpp \
        coordinatescene.cpp \
        main.cpp \
        mainwindow.cpp \
        myrectf.cpp \
        myshapef.cpp \
        mytrianglef.cpp \
        pointrecord.cpp \
        pointtableviewmodel.cpp \
        sceneellipse.cpp \
        sceneline.cpp \
        sceneobject.cpp \
        scenepoint.cpp \
        scenerectangle.cpp \
        scenetriangle.cpp \
        taskpopup.cpp \
        test.cpp \
        testparser.cpp

    HEADERS += \
        addrecorddialog.h \
        coordinatescene.h \
        mainwindow.h \
        myrectf.h \
        myshapef.h \
        mytrianglef.h \
        pointrecord.h \
        pointtableviewmodel.h \
        sceneellipse.h \
        sceneline.h \
        sceneobject.h \
        scenepoint.h \
        scenerectangle.h \
        scenetriangle.h \
        taskpopup.h \
        test.h \
        testparser.h

    FORMS += \
        addrecorddialog.ui \
        mainwindow.ui \
        taskpopup.ui
}

CONFIG(debug, debug|release|unit_tests) {
	message(debug build!)
	CONFIG += common
	TARGET = lab_01_29_debug
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

CONFIG(release, debug|release|unit_tests) {
	CONFIG += common
	message(release build!)

	TARGET = lab_01_29
}

# Default rules for deployment.

contains(CONFIG, unit_tests) {
    message(unit build!)
    CONFIG += testcase

    TARGET = unit_tests

    HEADERS += \
        mytrianglef.h \
        myshapef.h

    SOURCES += \
        test_mytrianglef.cpp \
        mytrianglef.cpp \
        myshapef.cpp \

    QT += testlib
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS += --coverage
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
