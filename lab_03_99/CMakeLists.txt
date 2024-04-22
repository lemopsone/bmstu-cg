cmake_minimum_required(VERSION 3.16)
project(lab_03_99 VERSION 1.0 LANGUAGES C CXX)


#set(CMAKE_CXX_COMPILER /opt/homebrew/bin/gcc)
set(CMAKE_CXX_FLAGS -Wall)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)
qt_standard_project_setup()

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR)
include_directories(${CMAKE_BINARY_DIR})

set(COMMON_SOURCES
        coordinatescene.cpp coordinatescene.h
        mainwindow.cpp mainwindow.h mainwindow.ui
        sceneline.cpp sceneline.h
        sceneobject.cpp sceneobject.h
        taskpopup.cpp taskpopup.h taskpopup.ui
        test.cpp test.h
        testparser.cpp testparser.h
)

add_executable(release ${COMMON_SOURCES} main.cpp
    linedrawer.h linedrawer.cpp)
add_executable(func ${COMMON_SOURCES} main.cpp)
add_executable(unit ${COMMON_SOURCES}
)

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
