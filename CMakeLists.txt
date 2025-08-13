cmake_minimum_required(VERSION 3.5)

project(Xrecorder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(X11 REQUIRED)

include_directories(${X11_INCLUDE_DIR})

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
	resources.qrc
	funcs/scr.cpp
	funcs/show_cursor.cpp
	funcs/scr.h
	funcs/vid.cpp
	funcs/vid.h
	funcs/settings.cpp
	funcs/settings.h
	funcs/areaselector.cpp
	funcs/areaselector.h
	funcs/drawcursorcheckbox.cpp
	funcs/drawcursorcheckbox.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Xrecorder
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Xrecorder APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Xrecorder SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Xrecorder
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_sources(Xrecorder PRIVATE
	${RESOURCES_ADDED_SOURCES}
)

target_link_libraries(Xrecorder
	PRIVATE
	Qt5::Widgets
	${X11_LIBRARIES}
	Xfixes
)

set_target_properties(Xrecorder PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Xrecorder)
endif()
