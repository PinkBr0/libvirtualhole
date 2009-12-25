######################################################################################
# CMake directives
######################################################################################

#Require 2.6 or higher.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)

#See if we've got our cmake modules checked out. If not, advise to do so.
IF(EXISTS "${CMAKE_SOURCE_DIR}/../compily_buildd/cmake")
  SET(BUILDSYS_CMAKE_DIR "${CMAKE_SOURCE_DIR}/../compily_buildd/cmake")
  MESSAGE(STATUS "Using outside compily_buildd directory ${BUILDSYS_CMAKE_DIR}")
ELSEIF(EXISTS "${CMAKE_SOURCE_DIR}/compily_buildd/cmake")
  SET(BUILDSYS_CMAKE_DIR "${CMAKE_SOURCE_DIR}/compily_buildd/cmake")
  MESSAGE(STATUS "Using compily_buildd git submodule ${BUILDSYS_CMAKE_DIR}")
ELSE(EXISTS "${CMAKE_SOURCE_DIR}/compily_buildd/cmake")
  MESSAGE(FATAL_ERROR
    "Cannot find compily_buildd directory for np labs project compilation functions.\n"
    "Make sure you've either put the compily_buildd directory in the same root as your repository directory, or that you've used the compily_buildd submodule (git submodule update --init).\n"
    "NP Labs build repository is available at git://github.com/qdot/compily_buildd.git"
    )
ENDIF(EXISTS "${CMAKE_SOURCE_DIR}/../compily_buildd/cmake")

LIST(APPEND CMAKE_MODULE_PATH ${BUILDSYS_CMAKE_DIR})

INCLUDE( ${BUILDSYS_CMAKE_DIR}/BuildSysCMakeLib.cmake )

#setting link directory policy

IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

######################################################################################
# Project declaration and options
######################################################################################

#Project declaration

PROJECT(LIBVIRTUALHOLE)
INITIALIZE_BUILD()

#Common Options

OPTION_LIBRARY_BUILD_STATIC(ON)
#We can't build shared on MSVC yet
IF(NOT MSVC)
  OPTION_LIBRARY_BUILD_SHARED(ON)
ENDIF(NOT MSVC)
OPTION_FAST_MATH(ON)
OPTION_ARCH_OPTS(ON)
OPTION_GPROF(OFF)

#Project specific options
OPTION(BUILD_EXAMPLES "Build libvirtualhole examples" ON)

######################################################################################
# Project specific package finding
######################################################################################

# IF(NOT WIN32)
#   FIND_PACKAGE(libusb-1.0 REQUIRED)
#   INCLUDE_DIRECTORIES(${LIBUSB_1_INCLUDE_DIRS})
#   SET(LIBVIRTUALHOLE_DEP_LIBS ${LIBUSB_1_LIBRARIES})
# ELSE()
# ENDIF()
IF(NOT WIN32)
  FIND_PACKAGE(libftdi REQUIRED)
  FIND_PACKAGE(libusb REQUIRED)
  INCLUDE_DIRECTORIES(${LIBUSB_INCLUDE_DIRS} ${LIBFTDI_INCLUDE_DIRS})
  SET(LIBVIRTUALHOLE_DEP_LIBS ${LIBFTDI_LIBRARIES} ${LIBUSB_LIBRARIES})
ENDIF()

######################################################################################
# Project specific globals
######################################################################################

#library definitions

SET(LIBVIRTUALHOLE_MAJOR_VERSION 1)
SET(LIBVIRTUALHOLE_MINOR_VERSION 1)
SET(LIBVIRTUALHOLE_BUILD_VERSION 0)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

SET(LIBVIRTUALHOLE_VERSION ${LIBVIRTUALHOLE_MAJOR_VERSION}.${LIBVIRTUALHOLE_MINOR_VERSION}.${LIBVIRTUALHOLE_BUILD_VERSION})

#library name definitions

CREATE_LIBRARY_LINK_NAME(virtualhole)

INCLUDE_DIRECTORIES(${LIBVIRTUALHOLE_INCLUDE_DIR})
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

#If we build libusb staticly on apple, we need the proper frameworks
IF(BUILD_STATIC AND APPLE)
  LIST(APPEND LIBVIRTUALHOLE_DEP_LIBS "-framework IOKit" "-framework CoreFoundation")
ENDIF(BUILD_STATIC AND APPLE)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)
