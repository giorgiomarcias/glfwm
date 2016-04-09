GLFW Manager - C++ wrapper with multi-threading
===============================================

A [GLFW](http://www.glfw.org) C++ wrapper with additional OOP features.

### Features
* C++11
* wrapping of GLFW window-related concepts
* multiple event handling
* handling of same event with multiple handlers
* binding of same handler to multiple windows
* multiple content drawing
* drawing of the same content in multiple windows
* grouping of windows
* multi-threaded drawing with groups
* window-to-window update notifications
* update notifications to whole groups
* automatic control of the loop

### Compilation
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

### License
This software is subject to the [zlib/libpng](http://opensource.org/licenses/Zlib) License.
