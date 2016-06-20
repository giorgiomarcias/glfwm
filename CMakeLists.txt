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

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

# GLFW
FIND_PACKAGE(glfw3 3.2 REQUIRED)

OPTION(WITH_MULTITHREADING "Build GLFWM with multithreading facilities (i.e. thread-safe) or not." ON)

SET(HDR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
SET(HDRS
    ${HDR_DIR}/glfwm/common.hpp
    ${HDR_DIR}/glfwm/drawable.hpp
    ${HDR_DIR}/glfwm/enums.hpp
    ${HDR_DIR}/glfwm/event.hpp
    ${HDR_DIR}/glfwm/event_handler.hpp
    ${HDR_DIR}/glfwm/update_map.hpp
    ${HDR_DIR}/glfwm/utility.hpp
    ${HDR_DIR}/glfwm/window.hpp
    ${HDR_DIR}/glfwm/window_group.hpp
    ${HDR_DIR}/glfwm/glfwm.hpp)

SET(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(SRCS
    ${SRC_DIR}/enums.cpp
    ${SRC_DIR}/event.cpp
    ${SRC_DIR}/update_map.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/window_group.cpp
    ${SRC_DIR}/glfwm.cpp)

ADD_LIBRARY(GLFWM ${HDRS} ${SRCS})

SET_TARGET_PROPERTIES(GLFWM PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

TARGET_INCLUDE_DIRECTORIES(GLFWM PUBLIC ${HDR_DIR}/glfwm)
TARGET_LINK_LIBRARIES(GLFWM glfw)
IF(NOT WITH_MULTITHREADING)
    TARGET_COMPILE_DEFINITIONS(GLFWM PRIVATE NO_MULTITHREADING)
ENDIF(NOT WITH_MULTITHREADING)

# expose the directories
SET(GLFWM_INCLUDE_DIRS ${HDR_DIR} PARENT_SCOPE)
SET(GLFWM_LIBRARIES GLFWM PARENT_SCOPE)
