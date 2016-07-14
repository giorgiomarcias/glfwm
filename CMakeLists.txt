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

project(glfwm VERSION 3.2 LANGUAGES CXX)

# GLFW
find_package(glfw3 3.2 REQUIRED)



get_directory_property(GLFWM_PARENT_DIRECTORY PARENT_DIRECTORY)



option(WITH_MULTITHREADING "Build GLFWM with multithreading facilities (i.e. thread-safe) or not." ON)

if(GLFWM_PARENT_DIRECTORY)
    set(MAKE_SHARED OFF)
else(GLFWM_PARENT_DIRECTORY)
    set(MAKE_SHARED ON)
endif(GLFWM_PARENT_DIRECTORY)
option(BUILD_SHARED_LIBS "Build GLFWM as a shared library or a static library." ${MAKE_SHARED})

if(GLFWM_PARENT_DIRECTORY)
    set(MAKE_INSTALL OFF)
else(GLFWM_PARENT_DIRECTORY)
    set(MAKE_INSTALL ON)
endif(GLFWM_PARENT_DIRECTORY)
option(INSTALL_GLFWM "Specify whether installing GLFWM on the system or not" ${MAKE_INSTALL})

if(NOT GLFWM_PARENT_DIRECTORY AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug|Release|RelWithDebInfo|MinSizeRel." FORCE)
endif(NOT GLFWM_PARENT_DIRECTORY AND NOT CMAKE_BUILD_TYPE)


set(HDR_DIR_NAME "GLFWM")
set(HDR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(HDRS
    ${HDR_DIR}/${HDR_DIR_NAME}/common.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/drawable.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/enums.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/event.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/event_handler.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/update_map.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/utility.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/window.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/window_group.hpp
    ${HDR_DIR}/${HDR_DIR_NAME}/glfwm.hpp)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SRCS
    ${SRC_DIR}/enums.cpp
    ${SRC_DIR}/event.cpp
    ${SRC_DIR}/update_map.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/window_group.cpp
    ${SRC_DIR}/glfwm.cpp)

add_library(${PROJECT_NAME} ${HDRS} ${SRCS})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PUBLIC ${HDR_DIR})
target_link_libraries(${PROJECT_NAME} glfw)
if(NOT WITH_MULTITHREADING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NO_MULTITHREADING)
endif(NOT WITH_MULTITHREADING)

# expose the directories
if(GLFWM_PARENT_DIRECTORY)
    set(GLFWM_TARGET ${PROJECT_NAME} PARENT_SCOPE)
    set(GLFWM_INCLUDE_DIRS ${HDR_DIR} PARENT_SCOPE)
    set(GLFWM_LIBRARIES ${PROJECT_NAME} PARENT_SCOPE)
endif(GLFWM_PARENT_DIRECTORY)

if(INSTALL_GLFWM)
    install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
    install(DIRECTORY ${HDR_DIR}/${HDR_DIR_NAME} DESTINATION include)
endif(INSTALL_GLFWM)
