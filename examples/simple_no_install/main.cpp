// Copyright (c) 2015-2016 Giorgio Marcias
//
// This file is part of GLFWM, a C++11 wrapper of GLFW with
// multi-threading management (GLFW Manager).
//
// This source code is subject to zlib/libpng License.
// This software is provided 'as-is', without any express
// or implied warranty. In no event will the authors be held
// liable for any damages arising from the use of this software.
//
// Author: Giorgio Marcias
// email: marcias.giorgio@gmail.com



// glfwmExample
//
// This example basically shows how to use GLFWM.
// First of all, you must have GLFW installed (see http://www.glfw.org ).
// This software works as follows.
// A first window is created. Whenever a left-click on it occurs, a new
// window is created from that taking the 90% of its space, starting from
// the bottom-right corner.
// Any left-click from any window causes the creation of a new window as
// before, from the window where the click occurred.
// A right-click on a window closes that window, without affecting the ones
// that have previously been created from that.
// When all windows are closed, the program ends.
// Any click on a window causes also a drawing, which consists of writing
// "Drawing window #" on the standard outputs, where '#' correponds to the
// ID of the window to be drawn.
//
// A more advanced usage of GLFWM implies a good knowledge of GLFW and can
// be achieved by reading the documentation in the header files.



#include <iostream>

// Before the main header, if not in the CMakefile.txt, define any GLFW macro you need, e.g.:
//#define GLFW_INCLUDE_GLCOREARB

// Include the main header.
// Depending on what you have to do (e.g. inherit some glfwm base class) you can include just any other header file.
#include <GLFWM/glfwm.hpp>

// This class responsible of drawing the content of a window must inherit glfwm::Drawable and
// implement its draw abstract method. Note that the parameter id can be used to retrieve
// that window specific resources (e.g. OpenGL buffers or VAOs).
class MyDrawable : public glfwm::Drawable {
public:
    void draw(const glfwm::WindowID id) override
    {
        // Here you can do anything you want for drawing, like glViewport(x, y, w, h).
        // Note: you don't have to call any swapping render buffer function, as it is
        // called when all the bound Drawables' draw method have been called (see
        // glfwm::WindowManager::mainLoop()).
        std::cout << "Drawing window " << id << std::endl;
    }
};
std::shared_ptr<MyDrawable> myDrawable;



class MyHandler;
std::shared_ptr<MyHandler> myHandler;

// This class is responsible of handling the events sent to the window(s) it is bound to.
// It must inherit glfwm::EventHandler and implement the getHandledEventTypes() and
// handle() methods.
class MyHandler : public glfwm::EventHandler {
public:
    // It is essential that this method returns a mask corresponding to
    // the event types that can be handled, otherwise the handle() method won't never be called
    // for the events not indicated, even if they can actually be handled.
    glfwm::EventBaseType getHandledEventTypes() const override
    {
        return static_cast<glfwm::EventBaseType>(glfwm::EventType::MOUSE_BUTTON);
    }

    // Then this method takes an action when the events that can be handled occur.
    // Specifically, when you left-click on a window, a new window is created, whilst
    // when you right-click on a window, it is closed without affecting those created
    // from that one.
    // PS: When several handlers that can handle the same event are bound to a window
    // and that event occurs, the handlers that come after the first one to handle it
    // are not executed. This is because this method returns true when it actually handles
    // the event. The order of handlers is determined by what you specify at binding time
    // (see glfwm::Window::bindEventHandler()). If you want to execute more than one handler
    // for the same event, you are free to return false even having actually handled it; in
    // this way, the following handlers are called.
    bool handle(const glfwm::EventPointer &e) override
    {
        if (e->getEventType() == glfwm::EventType::MOUSE_BUTTON) {

            std::shared_ptr<glfwm::EventMouseButton> mb = std::dynamic_pointer_cast<glfwm::EventMouseButton>(e);

            if (mb->getAction() != glfwm::ActionType::RELEASE)
                return false;

            glfwm::WindowPointer win = glfwm::Window::getWindow(e->getWindowID());
            int w, h, x, y;
            win->getSize(w, h);
            win->getPosition(x, y);

            if (mb->getMouseButton() == glfwm::MouseButtonType::MOUSE_BUTTON_LEFT) {
                glfwm::WindowPointer newWin = glfwm::WindowManager::createWindow(w * 0.9, h * 0.9, std::string(), getHandledEventTypes());
                newWin->setTitle(std::string("Window ") + std::to_string(newWin->getID()) + std::string(". Built from ") + std::to_string(e->getWindowID()));
                newWin->setPosition(x + w * 0.1, y + h * 0.1);
                newWin->bindEventHandler(myHandler, 0);
                newWin->bindDrawable(myDrawable, 0);
                glfwm::WindowGroupPointer grp = glfwm::WindowGroup::getGroup(glfwm::WindowGroup::getWindowGroup(e->getWindowID()));
                if (grp)
                    grp->attachWindow(newWin->getID());
                return true;
            } else if (mb->getMouseButton() == glfwm::MouseButtonType::MOUSE_BUTTON_RIGHT) {
                win->setShouldClose(true);
                return true;
            }
            return false;
        }

        return false;
    }
};



int main(int argc, char *argv[])
{
    // First thing to do: initialization!
    glfwm::WindowManager::init();

    // You can set any GLFW hint with this method:
    glfwm::WindowManager::setHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Remember to instantiate the handlers and the drawables to bind to the windows.
    // A handler can be bound to any number of different windows, and a window can have
    // any number of handlers bound to it.
    // The same holds for Drawables.
    myHandler = std::make_shared<MyHandler>();
    myDrawable = std::make_shared<MyDrawable>();

    // Create windows with the glfwm::WindowManager::createWindow() method.
    // Alternatively, call glfwm::Window::newWindow() and then glfwm::WindowManager::registerCallbacks().
    // At this time, remember to register the callbacks for the event you want to handle.
    glfwm::WindowPointer mainWin = glfwm::WindowManager::createWindow(800, 600, std::string(), myHandler->getHandledEventTypes());

    // You can set the title of the window, and other stuff: see glfwm::Window.
    mainWin->setTitle(std::string("Main Window ") + std::to_string(mainWin->getID()));

    // Remember to bind the handlers and the drawables. The second parameter is the rank that determines
    // the position in the list bound to the Window. The list is sorted accordingly:
    // a handler with rank 3 comes before one with 4 and after one with -100; two handlers with the same
    // rank appear one after the other depending on which is bound after.
    mainWin->bindEventHandler(myHandler, 0);
    mainWin->bindDrawable(myDrawable, 0);

    // Now, if you want to draw the windows' content in parallel, the best way to do it is
    // to create a group, attach windows to it, and make it run in a separate thread by
    // calling glfwm::WindowGroup::runLoopConcurrently(). Of course you don't have to.
    // If you want to keep everything in the main thread, groups may still be usefull,
    // for example to send update notifications to all the windows in the same group
    // (see glfwm::UpdateMap::notify()). You could also turn off the multi-threading
    // support at compile time through the option WITH_MULTITHREADING (see CMakeLists.txt).
    glfwm::WindowGroupPointer grp = glfwm::WindowGroup::newGroup();
    grp->attachWindow(mainWin->getID());
    grp->runLoopConcurrently();

    // When at least one Window has been created, you can start the main loop.
    // It consists of alternating event handling and content drawing.
    // There are two possibilities:
    // 1) wait (blocking) for any event to occur and then drawing after handling
    // 2) handling and then drawing, without waiting if no event is in the queue.
    // These two ways can be exchanged at any time (and from any thread) by calling
    // glfwm::WindowManager::setPoll().
    // The main loop will run until at least one window is open. When all windows
    // are closed, the mainLoop() method ends.
    // Note that a window is closed and others remain open, that window is destroyed.
    glfwm::WindowManager::mainLoop();

    // Finally, deletes all the resources: any group is destroyed and GLFW is terminated.
    glfwm::WindowManager::terminate();

    return 0;
}
