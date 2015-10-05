# glfwm
GLFWM: GLFW Manager - A C++11 wrapper of GLFW with multi-threading support.



This is meant to be used as a library.
It depends on GLFW (see http://www.glfw.org), which must be installed on the system.

To compile this software, import it in another software through cmake.
Supposing to have your software root cmake file at

    <path-to-your-software>/CMakeLists.txt

add GLFWM as subdirectory to it:

    #SET(WITH_MULTITHREADING OFF CACHE BOOL "Build GLFWM with multithreading facilities (i.e. thread-safe) or not.")
    ADD_SUBDIRECTORY(<path-to-glfwm> ${CMAKE_CURRENT_BINARY_DIR}/glfwm)
    ...
    TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${GLFWM_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${GLFWM_LIBRARIES})

Then it can be compiled with the commands:

    > cmake <path-to-your-software>
    > make

It has been tested on Mac OS X 10.9 - 10.10



License
This software is subject to the zlib/libpng License.
