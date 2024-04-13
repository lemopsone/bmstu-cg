cmake_minimum_required(VERSION 3.16)
project(lab_01_29 VERSION 1.0 LANGUAGES C CXX)


#set(CMAKE_CXX_COMPILER /opt/homebrew/bin/gcc)
set(CMAKE_CXX_FLAGS -Wall)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)
qt_standard_project_setup()


message("here with ${CMAKE_BUILD_TYPE}")

#include_directories(
#        ${PROJECT_SOURCE_DIR}/include
#        ${PROJECT_SOURCE_DIR}/src
#)

#file(GLOB COMMON_SOURCES
#    "${PROJECT_SOURCE_DIR}/include/*.h"
#    "${PROJECT_SOURCE_DIR}/src/*.cpp"
#)
#LIST(REMOVE_ITEM COMMON_SOURCES
#    "${PROJECT_SOURCE_DIR}/src/main.cpp"
#)

set(COMMON_SOURCES
        addrecorddialog.cpp addrecorddialog.h addrecorddialog.ui
        coordinatescene.cpp coordinatescene.h
        mainwindow.cpp mainwindow.h mainwindow.ui
        myrectf.cpp myrectf.h
        myshapef.cpp myshapef.h
        mytrianglef.cpp mytrianglef.h
        pointrecord.cpp pointrecord.h
        pointtableviewmodel.cpp pointtableviewmodel.h
        sceneellipse.cpp sceneellipse.h
        sceneline.cpp sceneline.h
        sceneobject.cpp sceneobject.h
        scenepoint.cpp scenepoint.h
        scenerectangle.cpp scenerectangle.h
        scenetriangle.cpp scenetriangle.h
        taskpopup.cpp taskpopup.h taskpopup.ui
        test.cpp test.h
        testparser.cpp testparser.h
)

add_executable(release ${COMMON_SOURCES} main.cpp)
add_executable(func ${COMMON_SOURCES} main.cpp)
add_executable(unit ${COMMON_SOURCES} test_mytrianglef.cpp test_mytrianglef.h)

target_link_libraries(release PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

target_link_libraries(func PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

target_link_libraries(unit PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Test
)

target_compile_options(unit PRIVATE -g -fprofile-instr-generate -fcoverage-mapping -O0)

target_link_options(unit PRIVATE -fprofile-instr-generate)

target_compile_options(func PRIVATE -g -fprofile-instr-generate -fcoverage-mapping -O0)

target_link_options(func PRIVATE -fprofile-instr-generate)

get_target_property(COMPILE_OPTIONS unit COMPILE_OPTIONS)
message("Compile options: ${COMPILE_OPTIONS}")