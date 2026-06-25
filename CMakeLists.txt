cmake_minimum_required(VERSION 3.21)
project(DashboardApp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_DISABLE_PRECOMPILE_HEADERS ON)

# 解决 MSVC 中文编译错误
if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(DashboardApp
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    GaugeWidget.cpp
    GaugeWidget.h

    Dialog1.h
    Dialog1.cpp
    Dialog2.h
    Dialog2.cpp
    Dialog3.h
    Dialog3.cpp
    MyDialog.cpp
	Dialog1.h
	Dialog1.cpp
    DialogX.h
    DialogX.cpp
	Dialog2.h
	Dialog2.cpp
    MyDialog.h
    MyDialog_press.cpp
    MyDialog_press.h
    numpad.cpp
    numpad.h
    groupdialog.h
    groupdialog.cpp
    numpaddialog.cpp
    numpaddialog.h
    MyDialog.ui 
)
target_compile_features(DashboardApp PRIVATE cxx_std_17)

target_link_libraries(DashboardApp PRIVATE Qt6::Widgets)

if(WIN32)
    set_target_properties(DashboardApp PROPERTIES WIN32_EXECUTABLE TRUE)
endif() 