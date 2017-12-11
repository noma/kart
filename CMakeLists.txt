# Copyright (c) 2015-2017 Matthias Noack (ma.noack.pr@gmail.com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(kart_project)

option(KART_ENABLE_FORTRAN "Enable FORTRAN support" OFF)
if(KART_ENABLE_FORTRAN)
	enable_language(Fortran)
endif()


# libkart name
set(LIBRARY_NAME kart)
set(LIBRARY_NAME_C kartc)

# location of all thirdparty components
set(THIRDPARTY_DIR ${PROJECT_SOURCE_DIR}/thirdparty)

# location of documentation
set(DOC_DIR ${PROJECT_SOURCE_DIR}/doc)

# set output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
message("CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})

# compiler options
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# allow cross-compiling for Intel Xeon Phi (Knights Corner) accelerators
# TODO: check for intel compiler
if(KART_ENABLE_MIC_BUILD)
	MESSAGE("Cross-compiling for MIC, make sure to use a suitable (Intel) compiler by exporting CC and CXX.")
	list(APPEND CMAKE_CXX_FLAGS "-mmic" ${CMAKE_CXX_FLAGS})
	list(APPEND CMAKE_LD_FLAGS "-mmic" ${CMAKE_LD_FLAGS})
endif()

find_package(Boost REQUIRED COMPONENTS program_options system filesystem iostreams)
add_definitions(-DBOOST_FILESYSTEM_NO_DEPRECATED)

# project wide include dir
include_directories(${PROJECT_SOURCE_DIR}/include ${THIRDPARTY_DIR}/process ${Boost_INCLUDE_DIRS})

# see CMakeLists.txt in src
add_subdirectory(src)
#add_subdirectory(thirdparty/bmt)

# Doxygen target 'make doc'
find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${DOC_DIR}/Doxyfile WORKING_DIRECTORY ${DOC_DIR} COMMENT "Running Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)
