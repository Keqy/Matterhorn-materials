cmake_minimum_required(VERSION 3.16)
project(Matterhorn-materials VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/sources/gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)

set(PROJECT_SOURCES
        ${CMAKE_SOURCE_DIR}/sources/main.cpp
        ${CMAKE_SOURCE_DIR}/sources/mainwindow.cpp
        ${CMAKE_SOURCE_DIR}/sources/include/mainwindow.h
        ${CMAKE_SOURCE_DIR}/sources/gui/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Matterhorn-materials
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${CMAKE_SOURCE_DIR}/resources/resources.qrc
        ${CMAKE_SOURCE_DIR}/sources/include/materialswindow.h
        ${CMAKE_SOURCE_DIR}/sources/materialswindow.cpp
        ${CMAKE_SOURCE_DIR}/sources/gui/materialswindow.ui
        ${CMAKE_SOURCE_DIR}/sources/include/databasemanager.h
        ${CMAKE_SOURCE_DIR}/sources/databasemanager.cpp
        ${CMAKE_SOURCE_DIR}/sources/include/editmaterialstreewidgetdialog.h
        ${CMAKE_SOURCE_DIR}/sources/editmaterialstreewidgetdialog.cpp
        ${CMAKE_SOURCE_DIR}/sources/gui/editmaterialstreewidgetdialog.ui
        ${CMAKE_SOURCE_DIR}/sources/include/renamenewmaterialstreewidgetitemdialog.h
        ${CMAKE_SOURCE_DIR}/sources/renamenewmaterialstreewidgetitemdialog.cpp
        ${CMAKE_SOURCE_DIR}/sources/gui/renamenewmaterialstreewidgetitemdialog.ui
        ${CMAKE_SOURCE_DIR}/sources/include/treechangetracker.h
        ${CMAKE_SOURCE_DIR}/sources/treechangetracker.cpp
        ${CMAKE_SOURCE_DIR}/sources/include/addmaterialdialog.h
        ${CMAKE_SOURCE_DIR}/sources/addmaterialdialog.cpp
        ${CMAKE_SOURCE_DIR}/sources/gui/addmaterialdialog.ui
        ${CMAKE_SOURCE_DIR}/sources/include/material.h
        ${CMAKE_SOURCE_DIR}/sources/include/logindialog.h
        ${CMAKE_SOURCE_DIR}/sources/logindialog.cpp
        ${CMAKE_SOURCE_DIR}/sources/gui/logindialog.ui
        ${CMAKE_SOURCE_DIR}/sources/include/addextramaterialoptiondialog.h
        ${CMAKE_SOURCE_DIR}/sources/addextramaterialoptiondialog.cpp
        ${CMAKE_SOURCE_DIR}/sources/gui/addextramaterialoptiondialog.ui
        ${CMAKE_SOURCE_DIR}/sources/include/addextramaterialoptiondialog.h
        ${CMAKE_SOURCE_DIR}/sources/include/extramaterialoption.h
        ${CMAKE_SOURCE_DIR}/sources/include/confirmationmessagebox.h
    )
else()
    if(ANDROID)
        add_library(Matterhorn-materials SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Matterhorn-materials
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Matterhorn-materials PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Matterhorn-materials PRIVATE Qt6::Sql)

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
