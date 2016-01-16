## Version: $Id$
##
######################################################################
##
### Commentary:
##
######################################################################
##
### Change Log:
##
######################################################################
##
### Code:

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
  dtkMonitoring
  dtkGuiSupport
  dtkWidgets)

target_link_libraries(${PROJECT_NAME}
  dtkLinearAlgebraSparse
  dtkGeometry)

if(DTK_BUILD_SUPPORT_PLOT)
  target_link_libraries(${PROJECT_NAME} dtkPlotSupport)
endif(DTK_BUILD_SUPPORT_PLOT)

qt5_use_modules(${PROJECT_NAME} Concurrent)
qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Declarative)
qt5_use_modules(${PROJECT_NAME} Gui)
qt5_use_modules(${PROJECT_NAME} Quick)
qt5_use_modules(${PROJECT_NAME} Network)
qt5_use_modules(${PROJECT_NAME} Widgets)
qt5_use_modules(${PROJECT_NAME} Xml)

## #################################################################
## Installation
## #################################################################

install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION modules)
install(TARGETS ${PROJECT_NAME}
   BUNDLE DESTINATION bin
  RUNTIME DESTINATION bin)

######################################################################
### CMakeLists.txt ends here
