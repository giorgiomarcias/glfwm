# Copyright (c) 2015-2024 Giorgio Marcias
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

cmake_minimum_required(VERSION 3.5)

project(glfwm LANGUAGES CXX)

# set current glfwm version
set(GLFWM_VERSION_MAJOR "3")
set(GLFWM_VERSION_MINOR "4")
set(GLFWM_VERSION_PATCH "0")
set(GLFWM_VERSION_TWEAK "")
set(GLFWM_VERSION "${GLFWM_VERSION_MAJOR}.${GLFWM_VERSION_MINOR}")
set(GLFWM_VERSION_FULL "${GLFWM_VERSION}.${GLFWM_VERSION_PATCH}${GLFWM_VERSION_TWEAK}")

# depending on whether this list file is included with add_subdirectory or not, behaviour changes
get_directory_property(GLFWM_PARENT_DIRECTORY PARENT_DIRECTORY)



# dependencies:

# GLFW
find_package(glfw3 3.4 REQUIRED)



# options:

option(WITH_MULTITHREADING "Build GLFWM with multithreading (i.e. thread-safe) or not." ON)

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


# collect source files
set(HDR_DIR_NAME "GLFWM")
set(HDR_DIR "${${PROJECT_NAME}_SOURCE_DIR}/include")
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

set(SRC_DIR "${${PROJECT_NAME}_SOURCE_DIR}/src")
set(SRCS
    ${SRC_DIR}/enums.cpp
    ${SRC_DIR}/event.cpp
    ${SRC_DIR}/update_map.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/window_group.cpp
    ${SRC_DIR}/glfwm.cpp)



# create the target library
add_library(${PROJECT_NAME} ${HDRS} ${SRCS})



# set target properties:

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    VERSION ${GLFWM_VERSION}
    SOVERSION ${GLFWM_VERSION_MAJOR}
)
if(NOT GLFWM_PARENT_DIRECTORY)
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif(NOT GLFWM_PARENT_DIRECTORY)

target_include_directories(${PROJECT_NAME} PRIVATE ${HDR_DIR})
target_include_directories(${PROJECT_NAME} INTERFACE
                           $<BUILD_INTERFACE:${HDR_DIR}>
                           $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
if(NOT WITH_MULTITHREADING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NO_MULTITHREADING)
endif(NOT WITH_MULTITHREADING)



# installation directives:
if(INSTALL_GLFWM)
    # first, install the target library
    install(TARGETS ${PROJECT_NAME}
            EXPORT ${PROJECT_NAME}Targets
            LIBRARY DESTINATION lib
            INCLUDES DESTINATION include # this appears in INTERFACE properties when exporting
    )
    # install the header files
    install(DIRECTORY ${HDR_DIR}/${HDR_DIR_NAME} DESTINATION include)

    # set and intall the cmake config files, s.t. consumers can easily find this library locations and properties
    set(GLFWM_CONFIG_PATH "lib/cmake/${PROJECT_NAME}")

    include(CMakePackageConfigHelpers)
    configure_package_config_file(cmake/glfwmConfig.cmake.in
                                  ${PROJECT_NAME}Config.cmake
                                  INSTALL_DESTINATION "${GLFWM_CONFIG_PATH}"
                                  NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )
    write_basic_package_version_file(${PROJECT_NAME}ConfigVersion.cmake
                                     VERSION ${GLFWM_VERSION_FULL}
                                     COMPATIBILITY SameMajorVersion
    )
    install(FILES
            ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake
            ${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
            DESTINATION "${GLFWM_CONFIG_PATH}"
    )

    install(EXPORT ${PROJECT_NAME}Targets
            DESTINATION ${GLFWM_CONFIG_PATH}
            FILE ${PROJECT_NAME}Targets.cmake
            EXPORT_LINK_INTERFACE_LIBRARIES
    )
endif(INSTALL_GLFWM)
