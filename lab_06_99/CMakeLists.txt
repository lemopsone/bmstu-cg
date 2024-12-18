cmake_minimum_required(VERSION 3.16)
project(lab_06_99 VERSION 1.0 LANGUAGES C CXX)


#set(CMAKE_CXX_COMPILER /usr/bin/clang)
set(CMAKE_CXX_FLAGS -Wall)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Test Widgets)
qt_standard_project_setup()

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR)
include_directories(${CMAKE_BINARY_DIR})

set(COMMON_SOURCES
        coordinatescene.cpp coordinatescene.h
        mainwindow.cpp mainwindow.h mainwindow.ui
        taskpopup.cpp taskpopup.h taskpopup.ui
        test.cpp test.h
        testparser.cpp testparser.h
        pointtableviewmodel.h
        pointtableviewmodel.cpp
        pointrecord.h pointrecord.cpp
        fill.cpp fill.h
)

add_executable(release ${COMMON_SOURCES} main.cpp)
add_executable(func ${COMMON_SOURCES} main.cpp)

add_executable(unit ${COMMON_SOURCES} testfill.cpp testfill.h)

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

target_compile_options(func PRIVATE -g -fprofile-instr-generate -fcoverage-mapping -O1)
target_link_options(func PRIVATE -fprofile-instr-generate)
