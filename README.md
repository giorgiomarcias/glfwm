GLFWM

GLFW Manager - A C++11 wrapper of GLFW with multi-threading support.



This is meant to be used as a library.
It depends on GLFW (see http://www.glfw.org), which must be installed on the system.



Features:
- wrapping of GLFW window-related concepts
- multiple event handling through class derivation
- handling of the same event with multiple handlers
- handling of different events for different windows with the same handler
- multiple content drawing through class derivation
- drawing of the same content in multiple windows
- grouping of windows
- multi-threaded drawing through groups
- sending update notifications to any window from others
- update notifications to whole groups
- automatic control of the loop



Compilation:

To compile this software, import it in another software through cmake.
Supposing to have your software root cmake file at

    <path-to-your-software>/CMakeLists.txt

add GLFWM as subdirectory to it:

    # uncomment the following line if you do NOT want multi-threading support
    #SET(WITH_MULTITHREADING OFF CACHE BOOL "Build GLFWM with multithreading facilities (i.e. thread-safe) or not.")
    ADD_SUBDIRECTORY(<path-to-glfwm> ${CMAKE_CURRENT_BINARY_DIR}/glfwm)
    ...
    TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${GLFWM_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GLFWM_LIBRARIES})

Then it can be compiled with the commands:

    > cmake <path-to-your-software>
    > make

It has been tested on Mac OS X 10.9 - 10.10 - 10.11



License

This software is subject to the zlib/libpng License.
