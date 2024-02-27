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
#### Dependencies
There are two mandatory dependencies.

* [GLFW](http://www.glfw.org), which must have already been installed on the system.
See [http://www.glfw.org](http://www.glfw.org) for details.
Alternatively, it is possible to get GLFW through package managers, such as apt-get (Linux) or MacPort/Homebrew (Mac OS).

* [cmake](https://cmake.org), which is used to compile and install this library, greatly simplifying this tasks.
See [https://cmake.org](https://cmake.org) for details.
Alternatively, as before, it is possible to get cmake through package managers.



#### Building
Through *cmake* it is possible to compile and eventually import this library into another software.
There are two ways to do that.

* *install* the shared library binaries somewhere on the system and then import it:

        mkdir glfwm_build
        cd glfwm_build
        cmake <path-to-glfwm>
        make
        make install

    Now `glfwm` is installed as a shared library depending on the value of `CMAKE_INSTALL_PREFIX` and can be linked to.
    Then in the cmake list file `CMakeList.txt` of another project just add:

        ...
        find_package(glfwm 3.3 REQUIRED)
        ...
        add_executable(myexe myexe.cpp)
        target_link_libraries(myexe glfwm)

    And that's all.

* *include* the library sources in a cmake list file `CMakeList.txt` of another project:

        ...
        add_subdirectory(<path-to-glfwm> ${${PROJECT_NAME}_BINARY_DIR}/glfwm)
        ...
        add_executable(myexe myexe.cpp)
        target_link_libraries(myexe glfwm)

    In this way, `glfwm` is compiled as a static library in the build tree of the `myexe` target.

##### Build options
There are some options that affect the compilation and/or installation:

* `WITH_MULTITHREADING` enables/disables multi-threading support. This allows windows to operate and be managed in separate threads.

* `BUILD_SHARED_LIBS` makes `glfwm` be built as a shared (if `ON`) or a static (if `OFF`) library.

* `INSTALL_GLFWM` makes `glfwm` be installed (if `ON`) or not (if `OFF`).

Default values mainly depends on the way it is built as described in the **Building** section above.
`BUILD_SHARED_LIBS` and `INSTALL_GLFWM` are `OFF` when glfwm is included as a sub-directory, `ON` otherwise.
`WITH_MULTITHREADING` is always `ON`.

It is possible to change these options either as argument to the `cmake` command, e.g. `-DWITH_MULTITHREADING=OFF`, or directly in the cmake list file of another project which includes glfwm:

    ...
    option(WITH_MULTITHREADING "Build GLFWM with multithreading (i.e. thread-safe) or not." OFF)
    add_subdirectory(<path-to-glfwm> ${${PROJECT_NAME}_BINARY_DIR}/glfwm)
    ...

### Usage
The main purpose is to manage windows.
So the main objects are `Window`s and a `WindowManager` which takes care of.

First of all, include the main header:

    #include <GLFWM/glfwm.hpp>

Then initialize the library resources by calling

    glfwm::WindowManager::init();

at the beginning of the program execution, e.g. in the `main` function before everything.

A `Window` can receive events and draw some content.
For this purpose, special objects can bind to it.

Derive `EventHandler` and override the `getHandledEventTypes()` and `handle()` methods in order to react to events like cursor position changes and mouse button clicks.
Then bind such objects to a `Window` s.t. events directed to it may handled:

    class MyHandler : public glfwm::EventHandler {
        public:
        glfwm::EventBaseType getHandledEventTypes() const override
        {
            return static_cast<glfwm::EventBaseType>(glfwm::EventType::MOUSE_BUTTON);
        }

        bool handle(const glfwm::EventPointer &e) override
        {
            if (e->getEventType() == glfwm::EventType::MOUSE_BUTTON) {
                // make some action based on this event
                ...
                return true;
            }
            return false;
        }
    };

Derive `Drawable` and override the `draw()` method.
There put the code responsible of drawing the content, e.g. OpenGL calls:

    class MyDrawable : public glfwm::Drawable {
        public:
        void draw(const glfwm::WindowID id) override
        {
            // draw the content on the screen
        }
    };

Then create some `Window` and bind the handler and the drawable to it:

    std::shared_ptr<MyHandler> myHandler = std::make_shared<MyHandler>();
    std::shared_ptr<MyDrawable> myDrawable = std::make_shared<MyDrawable>();
    glfwm::WindowPointer mainWin = glfwm::WindowManager::createWindow(800, 600, std::string(), myHandler->getHandledEventTypes());
    mainWin->bindEventHandler(myHandler, 0);    // 0 is the rank among all event handlers bound
    mainWin->bindDrawable(myDrawable, 0);       // 0 is the rank among all drawables bound

Possibly create other windows and group them.
Groups are useful for concurrent management (i.e. multi-threaded windows) or even just for sending notifications to all the windows in the same group.
Notifications can be used to make several windows react to a single event.
To create a group and attach windows:

    glfwm::WindowGroupPointer grp = glfwm::WindowGroup::newGroup();
    grp->attachWindow(mainWin->getID());
    grp->runLoopConcurrently();     // this is available only if compiled with WITH_MULTITHREADING=ON

Finally, start the main loop, which ends when all the windows are closed, and release the library resources:

    glfwm::WindowManager::mainLoop();
    glfwm::WindowManager::terminate();

See the examples for more details.

It has been tested on macOS's: 10.9 - 10.10 - 10.11 - 10.12 - 10.13 - 10.14 - 14.3

### License
This software is subject to the [zlib/libpng](http://opensource.org/licenses/Zlib) License.
