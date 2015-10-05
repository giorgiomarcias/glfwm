// Copyright (c) 2015 Giorgio Marcias
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

#include "window.hpp"

namespace glfwm {
    
    /**
     *  @brief  Contructor. This can be called only in the main thread.
     *  @param  id The ID of this new window.
     *  @param  width The width of the window area.
     *  @param  height The height of the window area.
     *  @param  title The title of the window.
     *  @param  monitor The GLFWMonitor to associate this window to in case of fullscreen. Use nullptr otherwise.
     *  @param  share Another Window to share the context with. Use nullptr for a non-shared context.
     *  @note   The creation of a GLFW window may occur only in the main thread. Do not instantiate a Window in secondary threads.
     */
    Window::Window(const WindowID id, const int width, const int height, const std::string &title, GLFWmonitor *monitor, const WindowPointer &share)
    : windowID(id)
#ifndef NO_MULTITHREADING
    , sharedMutexID(share ? share->sharedMutexID : newMutexID())
#endif
    {
        glfwWindow = glfwCreateWindow(width, height, title.c_str(), monitor, share ? share->glfwWindow : nullptr);
        if (!glfwWindow)
            throw std::runtime_error(std::string("Error. GLFW window not created."));
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        windowsMap.insert(std::make_pair(glfwWindow, id));
        
    }
    
    /**
     *  @brief  Destructor.
     */
    Window::~Window()
    {
        destroy();
    }
    
    /**
     *  @brief  The destroy method releases all GLFW data owned by this Window. This may only be called from the main thread.
     */
    void Window::destroy()
    {
#ifndef NO_MULTITHREADING
        if (sharedMutexID >= mutexes.size())
            return;     // probably this Window has been already destroied at this moment
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow) {
            unmapWindow(glfwWindow);
            glfwDestroyWindow(glfwWindow);
            glfwWindow = nullptr;
            freeWindowID(windowID);
#ifndef NO_MULTITHREADING
            decreaseMutexCount(sharedMutexID);
            sharedMutexID = std::numeric_limits<MutexID>::max();
#endif
        }
    }
    
    /**
     *  @brief  returns this Window's assigned ID.
     *  @return this Window's assigned ID.
     */
    WindowID Window::getID() const
    {
        return windowID;
    }
    
    /**
     *  @brief  The bindEventHandler method binds an EventHandler by adding it to the list of handlers in a position determined by the rank r.
     *  @param eh The EventHandler to bind.
     *  @param r  The rank associated to the EventHandler to bind. This determines its position in the list of handlers.
     *  @note   If the handler has been already bound, it is rebound with the eventually new rank.
     */
    void Window::bindEventHandler(const EventHandlerPointer &eh, const RankType r)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        
        EventHandlersIterator position;
        
        // first, look up the handler among those already bound
        EventHandlerMapIterator ehIt = eventHandlerMap.find(eh);
        
        // if bound, unbind it
        if (ehIt != eventHandlerMap.end()) {
            position = ehIt->second;
            eventHandlers.erase(position);
        } else {
            using EventHandlerMapInserResult = std::pair<EventHandlerMapIterator, bool>;
            EventHandlerMapInserResult res = eventHandlerMap.insert(std::make_pair(eh, eventHandlers.end()));
            ehIt = res.first;
        }
        
        // then (re)bind it
        EventHandlerRank ehr;
        ehr.object = eh;
        ehr.rank = r;
        position = std::lower_bound(eventHandlers.begin(), eventHandlers.end(), ehr);
        ehIt->second = eventHandlers.insert(position, ehr);
    }
    
    /**
     *  @brief  The unbindEventHandler methods removes the EventHandler eh from the list of handlers, if present.
     *  @param eh The EventHandler to unbind.
     */
    void Window::unbindEventHandler(const EventHandlerPointer &eh)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        
        // first, look up the handler among those already bound
        EventHandlerMapIterator ehIt = eventHandlerMap.find(eh);
        
        // if bound, unbind it
        if (ehIt != eventHandlerMap.end()) {
            // remove it from the list
            eventHandlers.erase(ehIt->second);
            // remove it from the map
            eventHandlerMap.erase(ehIt);
        }
    }
    
    /**
     *  @brief  The handleEvent method is called when an event occurs and it just calls the bound handlers in the order given by their rank until
     *          one of them positively handles it.
     *  @param e The event to handle.
     */
    void Window::handleEvent(const EventPointer &e)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        
        // check if this is the right recipient
        if (e->getWindowID() != windowID)
            return;
        
        // search the first handler that handles event e
        for (auto &h : eventHandlers)
            if (h.object->getHandledEventTypes() & e->getEventType() && h.object->handle(e))
                return;
    }
    
    /**
     *  @brief  The bindDrawable method binds a Drawable by adding it to the list of drawable objects in a position determined by the rank r.
     *  @param d The Drawable to bind.
     *  @param r  The rank associated to the Drawable to bind. This determines its position in the list of drawables.
     *  @note   If the drawable has been already bound, it is rebound with the eventually new rank. Moreover, a drawable may be bound to several windows.
     */
    void Window::bindDrawable(const DrawablePointer &d, const RankType r)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        
        DrawablesIterator position;
        
        // first, look up the drawable among those already bound
        DrawableMapIterator dIt = drawableMap.find(d);
        
        // if bound, unbind it
        if (dIt != drawableMap.end()) {
            position = dIt->second;
            drawables.erase(position);
        } else {
            using DrawableMapInserResult = std::pair<DrawableMapIterator, bool>;
            DrawableMapInserResult res = drawableMap.insert(std::make_pair(d, drawables.end()));
            dIt = res.first;
        }
        
        // then (re)bind it
        DrawableRank dr;
        dr.object = d;
        dr.rank = r;
        position = std::lower_bound(drawables.begin(), drawables.end(), dr);
        dIt->second = drawables.insert(position, dr);
    }
    
    /**
     *  @brief  The unbindDrawable methods removes the Drawable d from the list of drawable objects, if present.
     *  @param d The Drawable to unbind.
     */
    void Window::unbindDrawable(const DrawablePointer &d)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        
        // first, look up the drawable among those already bound
        DrawableMapIterator dIt = drawableMap.find(d);
        
        // if bound, unbind it
        if (dIt != drawableMap.end()) {
            // remove it from the list
            drawables.erase(dIt->second);
            // remove it from the map
            drawableMap.erase(dIt);
        }
    }
    
    /**
     *  @brief  The draw method is called when this window is rendered and it just calls the bound drawables in the order given by their rank.
     */
    void Window::draw()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        // draw each drawable in sequence
        for (auto &d : drawables)
            d.object->draw(windowID);
    }
    
    /**
     *  @brief  The shouldClose method is a wrapper of glfwWindowShouldClose.
     *  @return true if this window should close, false otherwise.
     */
    bool Window::shouldClose() const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            return glfwWindowShouldClose(glfwWindow);
        return true;
    }
    
    /**
     *  @brief  The setShouldClose method is a wrapper of glfwSetWindowShouldClose.
     *  @param c true for yes, false for no.
     */
    void Window::setShouldClose(const bool c) {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwSetWindowShouldClose(glfwWindow, c ? GL_TRUE : GL_FALSE);
    }
    
    /**
     *  @brief  The setTitle method changes the current title.
     *  @param title The new window title.
     *  @note   This may only be called from the main thread.
     */
    void Window::setTitle(const std::string &title)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwSetWindowTitle(glfwWindow, title.c_str());
    }
    
    /**
     *  @brief  The getPosition method returns the x and y screen coordinate position.
     *  @param x The x coordinate.
     *  @param y The y coordintate.
     *  @note   This may only be called from the main thread.
     */
    void Window::getPosition(int &x, int &y) const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwGetWindowPos(glfwWindow, &x, &y);
    }
    
    /**
     *  @brief  The getPosition method sets the x and y screen coordinate position.
     *  @param x The x coordinate.
     *  @param y The y coordintate.
     *  @note   This may only be called from the main thread.
     */
    void Window::setPosition(const int x, const int y)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwSetWindowPos(glfwWindow, x, y);
    }
    
    /**
     *  @brief  The getSize method returns the width and height of this window area.
     *  @param width The width of this window area.
     *  @param height The height of this window area.
     *  @note   This may only be called from the main thread.
     */
    void Window::getSize(int &width, int &height) const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwGetWindowSize(glfwWindow, &width, &height);
    }
    
    /**
     *  @brief  The setSize method sets the width and height of this window area.
     *  @param width The width of this window area.
     *  @param height The height of this window area.
     *  @note   This may only be called from the main thread.
     */
    void Window::setSize(const int width, const int height)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwSetWindowSize(glfwWindow, width, height);
    }
    
    /**
     *  @brief  The getFramebufferSize method returns the width and height of this window framebuffer.
     *  @param width The width of this window framebuffer.
     *  @param height The height of this window framebuffer.
     *  @note   This may only be called from the main thread.
     */
    void Window::getFramebufferSize(int &width, int &height) const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwGetFramebufferSize(glfwWindow, &width, &height);
    }
    
    /**
     *  @brief  The getFrameSize method returns size of each of the frame edges of this window.
     *  @param left The left edge size.
     *  @param top The top edge size.
     *  @param right The right edge size.
     *  @param bottom The bottom edge size.
     *  @note   This may only be called from the main thread.
     */
    void Window::getFrameSize(int &left, int &top, int &right, int &bottom) const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwGetWindowFrameSize(glfwWindow, &left, &top, &right, &bottom);
    }
    
    /**
     *    @brief  The getCursorPosition method returns the cursor position, in screen coordinates, relative to
     *            the upper left corner of this window.
     *    @param x The x-coordinate of the cursor.
     *    @param y The y-coordinate of the cursor.
     */
    void Window::getCursorPosition(double &x, double &y) const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwGetCursorPos(glfwWindow, &x, &y);
    }
    
    /**
     *    @brief  The setCursorPosition method changes the cursor position, in screen coordinates, relative to
     *            the upper left corner of this window.
     *    @param x The new x-coordinate of the cursor.
     *    @param y The new y-coordinate of the cursor.
     */
    void Window::setCursorPosition(double x, double y)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwSetCursorPos(glfwWindow, x, y);
    }
    
    /**
     *  @brief  The iconify method minimizes this window.
     *  @note   This may only be called from the main thread.
     */
    void Window::iconify()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwIconifyWindow(glfwWindow);
    }
    
    /**
     *  @brief  The restore method restores this window, if minimized.
     *  @note   This may only be called from the main thread.
     */
    void Window::restore()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwRestoreWindow(glfwWindow);
    }
    
    /**
     *  @brief  The hide method hides this window, if shown and not fullscreen.
     *  @note   This may only be called from the main thread.
     */
    void Window::hide()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwHideWindow(glfwWindow);
    }
    
    /**
     *  @brief  The show method shows this window, if hidden.
     *  @note   This may only be called from the main thread.
     */
    void Window::show()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwShowWindow(glfwWindow);
    }
    
    /**
     *  @brief  The getMonitor method returns the GLFWMonitor this window uses in fullscreen.
     *  @return This window monitor.
     */
    GLFWmonitor * Window::getMonitor() const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            return glfwGetWindowMonitor(glfwWindow);
        return nullptr;
    }
    
    /**
     *  @brief  The getAttribute method returns this window attributes. See GLFW.
     *  @param  attribute The attribute to read.
     *  @return The value of attribute for this window.
     *  @note   This may only be called from the main thread.
     */
    int Window::getAttribute(const int attribute) const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            return glfwGetWindowAttrib(glfwWindow, attribute);
        return 0;
    }
    
    /**
     *  @brief  The getUserPointer method returns the user pointer for this window. See GLFW.
     *  @return The user pointer.
     */
    void * Window::getUserPointer() const
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            return glfwGetWindowUserPointer(glfwWindow);
        return nullptr;
    }
    
    /**
     *  @brief  The setUserPointer method sets the user pointer for this window. See GLFW.
     *  @param pointer The user pointer to set.
     */
    void Window::setUserPointer(void *pointer)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwSetWindowUserPointer(glfwWindow, pointer);
    }
    
    /**
     *  @brief  The swapBuffers method swaps the front and the back buffer.
     */
    void Window::swapBuffers()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
        if (glfwWindow)
            glfwSwapBuffers(glfwWindow);
    }
    
    /**
     *  @brief  The makeContextCurrent method makes the context associated to this window current.
     */
    void Window::makeContextCurrent()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        mutexes[sharedMutexID].mutex.lock();
#endif
        if (glfwWindow)
            glfwMakeContextCurrent(glfwWindow);
    }
    
    /**
     *  @brief  The doneCurrentContext method releases the resources held by makeContextCurrent.
     *  @note   A call to this may only follow a call to makeContextCurrent, without nesting, otherwise the behaviour is undefined.
     *          This method does nothing if NO_MULTITHREADING is defined.
     */
    void Window::doneCurrentContext()
    {
#ifndef NO_MULTITHREADING
        mutexes[sharedMutexID].mutex.unlock();
#endif
    }
    
    
    
#ifndef NO_MULTITHREADING
    /**
     *  @brief  Mutex used to guarantee correct concurrent management of static activities.
     */
    std::recursive_mutex                        Window::globalMutex;
    
    /**
     *  @brief  The container of mutexes.
     */
    std::deque<Window::MutexData>               Window::mutexes;
    
    /**
     *  @brief  The container for reusing freed MutexIDs. It is kept sorted.
     */
    std::deque<Window::MutexID>                 Window::freedMutexes;
    
    /**
     *  @brief  The newMutexID static method books a new or an old & freed ID for mutexes.
     *  @return The booked MutexID.
     */
    Window::MutexID Window::newMutexID()
    {
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
        MutexID id = mutexes.size();
        if (!freedMutexes.empty()) {
            id = freedMutexes.front();
            freedMutexes.pop_front();
        }
        if (id >= mutexes.size())
            mutexes.resize(id + 1);
        mutexes[id].count++;
        return id;
    }
    
    /**
     *  @brief  The decreaseMutexCount static method decreases the number of users of a mutex, eventually freeing it.
     *  @param id The ID of the mutex.
     */
    void Window::decreaseMutexCount(const MutexID id)
    {
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
        if (id >= mutexes.size() || mutexes[id].count == 0)
            return;
        mutexes[id].count--;
        if (mutexes[id].count == 0) {
            std::deque<MutexID>::iterator fIt = std::lower_bound(freedMutexes.begin(), freedMutexes.end(), id);
            if (fIt == freedMutexes.end() || *fIt != id)
                freedMutexes.insert(fIt, id);
        }
    }
#endif
    
    /**
     *  @brief  The map between GLFWWindow pointers and WindowIDs.
     */
    Window::WindowMap                           Window::windowsMap;
    
    /**
     *  @brief  The container for collecting Windows.
     */
    std::deque<WindowPointer>                   Window::windows;
    
    /**
     *  @brief  The container for reusing freed WindowIDs.
     */
    std::deque<WindowID>                        Window::freedWindowIDs;
    
    /**
     *  @brief  The newWindowID static method books a new or an old & freed ID for windows.
     *  @return The booked WindowID.
     */
    WindowID Window::newWindowID()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        WindowID id = windows.size();
        if (!freedWindowIDs.empty()) {
            id = freedWindowIDs.front();
            freedWindowIDs.pop_front();
        }
        if (id >= windows.size())
            windows.resize(id + 1);
        return id;
    }
    
    /**
     *  @brief  The newWindow static method allocates and creates a new Window, storing also internal pointers and information
     *          in order to easily retrieve it later.
     *  @param width   The width of the new window, in screen space.
     *  @param height  The height of the new window, in screen space.
     *  @param title   The title of the new window.
     *  @param monitor The monitor for a fullscreen window, or a null pointer for a non-fullscreen window.
     *  @param share   The Window to share its context with, or a null pointer otherwise.
     *  @return A WindowPointer to the new window.
     */
    WindowPointer Window::newWindow(const int width, const int height, const std::string &title, GLFWmonitor *monitor, const WindowPointer &share)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        WindowID id = newWindowID();
        windows[id] = std::make_shared<Window>(id, width, height, title, monitor, share);
        return windows[id];
    }
    
    /**
     *  @brief  The getWindow static method gives the WindowPointer to a Window given its ID.
     *  @param id The WindowID of the Window to access.
     *  @return The WindowPointer of the requested Window, or a null pointer if it does not exist.
     */
    WindowPointer Window::getWindow(const WindowID id)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        if (id < windows.size())
            return windows[id];
        return WindowPointer(nullptr);
    }
    
    /**
     *  @brief  The getWindowID static method gives the ID of a Window associated to a GLFWWindow object.
     *  @param w The GLFWWindow object pointer.
     *  @return The WindowPointer of the associated Window.
     */
    WindowID Window::getWindowID(GLFWwindow *w)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        WindowMapIterator wIt = windowsMap.find(w);
        if (wIt != windowsMap.end())
            return wIt->second;
        return AllWindowIDs;
    }
    
    /**
     *  @brief  The getALlWindowIDs static method returns the set of all the WindowID currently in use.
     *  @param wIDs The set of WindowIDs.
     */
    void Window::getAllWindowIDs(std::unordered_set<WindowID> &wIDs)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        wIDs.clear();
        for (auto &w : windows)
            if (w)
                wIDs.insert(w->windowID);
    }
    
    /**
     *  @brief  The isAnyWindowOpen static method says if there is any Window still open.
     *  @return true if is there any Window still open, false otherwise.
     */
    bool Window::isAnyWindowOpen()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        for (auto &w : windows)
            if (w && !w->shouldClose())
                return true;
        return false;
    }
    
    /**
     *  @brief  The windowsToClose static method returns a set of Windows that have the close flag on.
     *  @param wtc The set of WindowIDs of the Windows to close.
     */
    void Window::windowsToClose(std::unordered_set<WindowID> &wtc)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        wtc.clear();
        for (auto &w : windows)
            if (w && w->shouldClose())
                wtc.insert(w->windowID);
    }
    
    /**
     *  @brief  The deleteWindow static method destroys and removes the Window at id.
     *  @param id The ID of the Window to remove.
     */
    void Window::deleteWindow(const WindowID id)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        if (id < windows.size() && windows[id]) {
            windows[id]->destroy();
            windows[id].reset();
        }
    }
    
    /**
     *  @brief  The deleteAllWindows static method destroys all current windows and delete everything.
     */
    void Window::deleteAllWindows()
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        windowsMap.clear();
        windows.clear();
        freedWindowIDs.clear();
#ifndef NO_MULTITHREADING
        freedMutexes.clear();
        mutexes.clear();
#endif
    }
    
    /**
     *  @brief  The unmapWindow static method removes the association between a GLFWWindow object and a Window.
     *  @param w The GLFWWindow object to unmap.
     */
    void Window::unmapWindow(GLFWwindow *w)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        windowsMap.erase(w);
    }
    
    /**
     *  @brief  The freeWindowID frees an ID for a later reuse.
     *  @param id The ID to free.
     */
    void Window::freeWindowID(const WindowID id)
    {
#ifndef NO_MULTITHREADING
        // acquire ownership
        std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
        std::deque<WindowID>::iterator pos = std::lower_bound(freedWindowIDs.begin(), freedWindowIDs.end(), id);
        freedWindowIDs.insert(pos, id);
    }
    
}
