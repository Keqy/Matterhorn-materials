cmake_minimum_required(VERSION 3.16)

project(Matterhorn-materials VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS sources/gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)

set(PROJECT_SOURCES
        sources/main.cpp
        sources/mainwindow.cpp
        sources/include/mainwindow.h
        sources/gui/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Matterhorn-materials
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        resources/resources.qrc
        sources/include/materialswindow.h sources/materialswindow.cpp sources/gui/materialswindow.ui

        sources/include/databasemanager.h sources/databasemanager.cpp
        sources/include/editmaterialstreewidgetdialog.h sources/editmaterialstreewidgetdialog.cpp sources/gui/editmaterialstreewidgetdialog.ui
        sources/include/renamenewmaterialstreewidgetitemdialog.h sources/renamenewmaterialstreewidgetitemdialog.cpp sources/gui/renamenewmaterialstreewidgetitemdialog.ui
        sources/include/treechangetracker.h sources/treechangetracker.cpp
        sources/include/addmaterialdialog.h sources/addmaterialdialog.cpp sources/gui/addmaterialdialog.ui
        sources/include/material.h
        sources/include/logindialog.h sources/logindialog.cpp sources/gui/logindialog.ui
        sources/include/addextramaterialoptiondialog.h sources/addextramaterialoptiondialog.cpp sources/gui/addextramaterialoptiondialog.ui
        sources/include/extramaterialoption.h








    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Matterhorn-materials APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Matterhorn-materials SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Matterhorn-materials
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Matterhorn-materials PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Matterhorn-materials PRIVATE Qt6::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Matterhorn-materials)
endif()
set_target_properties(Matterhorn-materials PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Matterhorn-materials
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Matterhorn-materials)
endif()
