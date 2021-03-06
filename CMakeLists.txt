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

cmake_minimum_required(VERSION 2.8.12)

######################################################################

if(${CMAKE_MAJOR_VERSION} GREATER 2)
  cmake_policy(SET CMP0048 OLD)
endif(${CMAKE_MAJOR_VERSION} GREATER 2)

##################################################################

project(dtkVisualProgramming)

## ###################################################################
## Version setup
## ###################################################################

set(${PROJECT_NAME}_VERSION_MAJOR 1)
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(${PROJECT_NAME}_VERSION_PATCH 0)
set(${PROJECT_NAME}_VERSION
  ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_PATCH}.${${PROJECT_NAME}_VERSION_PATCH})

## ###################################################################
## Path setup
## ###################################################################

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

## #################################################################
## Install prefix
## #################################################################

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "${PROJECT_NAME} install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

## ###################################################################
## Build setup
## ###################################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GNUInstallDirs)
include(GenerateExportHeader)

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I/usr/lib/c++/v1")
endif(APPLE)

if(NOT APPLE AND NOT MSVC)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-unknown-pragmas")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wno-unknown-pragmas")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
  endif()
else(NOT APPLE AND NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif(NOT APPLE AND NOT MSVC)

if(CMAKE_SIZEOF_VOID_P MATCHES 8)
  set(${PROJECT_NAME}_PLATFORM_64 1)
else(CMAKE_SIZEOF_VOID_P MATCHES 8)
  set(${PROJECT_NAME}_PLATFORM_32 1)
endif(CMAKE_SIZEOF_VOID_P MATCHES 8)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

## ###################################################################
## Test setup
## ###################################################################

enable_testing()

## #################################################################
## Dependencies - external
## #################################################################

find_package(Qt5 REQUIRED COMPONENTS
  Core
  Widgets
  )

## #################################################################
## Dependencies - external
## #################################################################

find_package(dtk 1.0.0 REQUIRED)
include_directories(${dtk_INCLUDE_DIRS})

find_package(dtkLinearAlgebraSparse 0.1.1 REQUIRED)
include_directories(${dtkLinearAlgebraSparse_INCLUDE_DIRS})

find_package(dtkGeometry 0.1.1 REQUIRED)
include_directories(${dtkGeometry_INCLUDE_DIRS})

## #################################################################
## Dependencies - internal
## #################################################################

include_directories(${CMAKE_BINARY_DIR})
#include_directories(${CMAKE_SOURCE_DIR}/app)

## ###################################################################
## Input
## ###################################################################

add_subdirectory(app)

# ## #################################################################
# ## Applicative target
# ## #################################################################

# set(DTKLINEARALGEBRASPARSE_TARGETS ${DTKLINEARALGEBRASPARSE_TARGETS} ${PROJECT_NAME})

# ## #################################################################
# ## Target export
# ## #################################################################

# export(TARGETS ${DTKLINEARALGEBRASPARSE_TARGETS} FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Depends.cmake")

## ###################################################################
## Exporting
## ###################################################################

# export(PACKAGE ${PROJECT_NAME})

# configure_file(${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
#   "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
# configure_file(${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake.in
#   "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)

# install(FILES
#   "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
#   "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#   DESTINATION cmake)

# install(EXPORT dtkLinearAlgebraSparseDepends DESTINATION cmake)

## ###################################################################
## Beautifying
## ###################################################################

mark_as_advanced(${PROJECT_NAME}_VERSION_MAJOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_MINOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_BUILD)

mark_as_advanced(Qt5Concurrent_DIR)
mark_as_advanced(Qt5Core_DIR)
mark_as_advanced(Qt5Declarative_DIR)
mark_as_advanced(Qt5Gui_DIR)
mark_as_advanced(Qt5Network_DIR)
mark_as_advanced(Qt5Qml_DIR)
mark_as_advanced(Qt5Quick_DIR)
mark_as_advanced(Qt5QuickTest_DIR)
mark_as_advanced(Qt5Script_DIR)
mark_as_advanced(Qt5Test_DIR)
mark_as_advanced(Qt5Widgets_DIR)
mark_as_advanced(Qt5Xml_DIR)

mark_as_advanced(CMAKE_AR)
mark_as_advanced(CMAKE_BUILD_TYPE)
mark_as_advanced(CMAKE_INSTALL_PREFIX)
mark_as_advanced(CMAKE_OSX_ARCHITECTURES)
mark_as_advanced(CMAKE_OSX_DEPLOYMENT_TARGET)
mark_as_advanced(CMAKE_OSX_SYSROOT)

## ###################################################################
## Packaging
## ###################################################################

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")

include(CPack)

######################################################################
### CMakeLists.txt ends here
