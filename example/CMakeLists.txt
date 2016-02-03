# Copyright (c) 2015-2016 Giorgio Marcias
#
# This file is part of GLFWM, a C++11 wrapper of GLFW with
# multi-threading management (GLFW Manager).
#
# This source code is subject to zlib/libpng License.
# This software is provided 'as-is', without any express
# or implied warranty. In no event will the authors be held
# liable for any damages arising from the use of this software.
#
# Author: Giorgio Marcias
# email: marcias.giorgio@gmail.com

PROJECT(glfwmExample)
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

# GLFWM
# if you want to use glfwm without multi-threading, uncomment the following line
#SET(WITH_MULTITHREADING OFF CACHE BOOL "Build GLFWM with multithreading facilities (i.e. thread-safe) or not.")
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR}/glfwm)

# create the executable
ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

# link libraries
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${GLFWM_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GLFWM_LIBRARIES})
