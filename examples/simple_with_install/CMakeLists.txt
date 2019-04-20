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

# find GLFWM
find_package(glfwm 3.3 REQUIRED CONFIG)

# create the executable
add_executable(${PROJECT_NAME} main.cpp)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

# link libraries
target_link_libraries(${PROJECT_NAME} glfwm)
