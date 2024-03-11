QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    cancel.png

RESOURCES += \
    application.qrc
