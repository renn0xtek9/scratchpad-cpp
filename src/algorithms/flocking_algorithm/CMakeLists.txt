cmake_minimum_required (VERSION 3.16)

project (flocking_algorithm LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_AUTOUIC ON)

find_package (
  Qt6 REQUIRED
  COMPONENTS
    Core
    Gui
    Widgets
    3DCore
    3DRender
    3DExtras
    3DInput)

add_executable (flocking_algorithm main.cpp boid.cpp mainwindow.cpp
                                   mainwindow.h)

target_link_directories (flocking_algorithm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (
  flocking_algorithm
  PRIVATE
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    Qt6::3DCore
    Qt6::3DRender
    Qt6::3DExtras
    Qt6::3DInput)
