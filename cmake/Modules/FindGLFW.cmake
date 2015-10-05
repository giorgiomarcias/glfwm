# Copyright (c) 2015 Giorgio Marcias
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



# FindGLFW
# --------
#
# Find the GLFW library

FIND_PATH(GLFW_INCLUDE_DIRS
        GLFW/glfw3.h
    PATHS
        /usr/include
        /usr/local/include
        /usr/X11/include
        /opt/local/include
    )

FIND_LIBRARY(GLFW_LIBRARIES
        glfw glfw3
    PATHS
        /usr
        /usr/local
        /usr/X11
        /opt/local
    PATH_SUFFIXES
        a
        lib
        lib64
    )

IF(GLFW_INCLUDE_DIRS AND GLFW_LIBRARIES)
    SET(GLFW_FOUND TRUE)
ENDIF(GLFW_INCLUDE_DIRS AND GLFW_LIBRARIES)
