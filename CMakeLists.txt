#-------------------------------------------------------------------------------
#	CMAKE SETTINGS
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.5)
project(minesweeper LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#-------------------------------------------------------------------------------
#	PROJECT SETTINGS
#-------------------------------------------------------------------------------

include(gitInfo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Ensure the output directory exists
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(MSVC)
	add_compile_options(/MP)
endif(MSVC)

#-------------------------------------------------------------------------------
#	Qt Settings
#-------------------------------------------------------------------------------

if (WIN32)
	set(Qt6_DIR $ENV{Qt6_DIR})
	list(APPEND CMAKE_PREFIX_PATH ${Qt6_DIR})
	message(STATUS "Qt6 DIR: ${Qt6_DIR}")
endif (WIN32)

find_package(Qt6 6.2.4 REQUIRED COMPONENTS Core Gui Widgets StateMachine Concurrent Network)
if (Qt6Core_VERSION VERSION_GREATER_EQUAL 6.3.0)
	qt_standard_project_setup()
else ()
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTORCC ON)
	set(CMAKE_AUTOUIC ON)
endif ()

#-------------------------------------------------------------------------------
#	CONFIGURATION
#-------------------------------------------------------------------------------

set(APPLICATION_ORGANIZATION "Menari Softworks")
set(APPLICATION_ORGANIZATION_DOMAIN "https://github.com/nholthaus/")

#-------------------------------------------------------------------------------
#	SUBDIRECTORIES
#-------------------------------------------------------------------------------

add_subdirectory(src)