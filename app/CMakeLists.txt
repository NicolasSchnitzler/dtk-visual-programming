### CMakeLists.txt ---
##
## Author: Julien Wintz
## Copyright (C) 2008 - Julien Wintz, Inria.
## Created: Mon Aug  3 17:34:51 2009 (+0200)
## Version: $Id$
## Last-Updated: Fri Apr 27 21:31:54 2012 (+0200)
##           By: Julien Wintz
##     Update #: 171
######################################################################
##
### Commentary:
##
######################################################################
##
### Change log:
##
######################################################################

project(dtkVisualProgramming)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS_MOC
  dtkVisualProgrammingMainWindow.h
  dtkVisualProgrammingMainWindow_p.h)

set(${PROJECT_NAME}_SOURCES
  dtkVisualProgrammingMainWindow.cpp
  main.cpp)

if(APPLE)
set(${PROJECT_NAME}_SOURCES
  ${${PROJECT_NAME}_SOURCES}
  dtkVisualProgrammingMainWindow.mm)
endif(APPLE)

set(${PROJECT_NAME}_RESOURCES
  dtkVisualProgramming.qrc)

## #################################################################
## Build rules
## #################################################################

if(NOT MSVC)
add_definitions(-Wno-write-strings)
endif(NOT MSVC)

qt5_add_RESOURCES(${PROJECT_NAME}_SOURCES_QRC ${${PROJECT_NAME}_RESOURCES})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE WIN32
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES_QRC}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})


target_link_libraries(${PROJECT_NAME}
  dtkLog
  dtkCore
  dtkComposer
  dtkGuiSupport
  dtkWidgets)

if(DTK_BUILD_SUPPORT_PLOT)
  target_link_libraries(${PROJECT_NAME} dtkPlotSupport)
endif(DTK_BUILD_SUPPORT_PLOT)

if(APPLE)
target_link_libraries(${PROJECT_NAME}
  "-framework Cocoa")
endif(APPLE)

qt5_use_modules(${PROJECT_NAME} Concurrent)
qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Declarative)
qt5_use_modules(${PROJECT_NAME} Gui)
qt5_use_modules(${PROJECT_NAME} Quick)
qt5_use_modules(${PROJECT_NAME} Network)
qt5_use_modules(${PROJECT_NAME} Widgets)
qt5_use_modules(${PROJECT_NAME} Xml)

## #################################################################
## Source file layout in development environments like Visual Studio
## #################################################################

SOURCE_GROUP("Header Files" REGULAR_EXPRESSION .*\\.h\$)
SOURCE_GROUP("Generated Files" FILES ${${PROJECT_NAME}_SOURCES_MOC})

## #################################################################
## Installation
## #################################################################

install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
install(TARGETS ${PROJECT_NAME}
   BUNDLE DESTINATION bin
  RUNTIME DESTINATION bin)
