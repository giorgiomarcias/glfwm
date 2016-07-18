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

cmake_minimum_required(VERSION 3.1)
project(glfwmExample LANGUAGES CXX)

# GLFWM
# if you want to use glfwm without multi-threading, uncomment the following line
#option(WITH_MULTITHREADING "Build GLFWM with multithreading facilities (i.e. thread-safe) or not." OFF)
add_subdirectory(${${PROJECT_NAME}_SOURCE_DIR}/../.. ${${PROJECT_NAME}_BINARY_DIR}/glfwm)

# create the executable
add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

# link libraries
target_link_libraries(${PROJECT_NAME} glfwm)
