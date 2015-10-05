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

#ifndef GLFWM_WINDOW_HPP
#define GLFWM_WINDOW_HPP

#include "event_handler.hpp"
#include "drawable.hpp"

namespace glfwm {
    
    class Window;
    
    /**
     *  @brief  The WindowPointer is a smart pointer for a Window.
     */
    using   WindowPointer               =   std::shared_ptr<Window>;
    
    /**
     *  @brief  The Window class represents a GLFWWindow with a set of threading facilities and some handlers for interactive objects, like event handlers or drawables.
     */
    class Window {
    public:
        /**
         *  @brief  The type used for the rank determining the position in lists.
         */
        using   RankType                =   int;
        
        /**
         *  @brief  Contructor. See newWindow for a reliable construction.
         *  @param  id The ID of this new window. See newWindowID.
         *  @param  width The width of the window area.
         *  @param  height The height of the window area.
         *  @param  title The title of the window.
         *  @param  monitor The GLFWMonitor to associate this window to in case of fullscreen. Use nullptr otherwise.
         *  @param  share Another Window to share the context with. Use nullptr for a non-shared context.
         *  @note   The creation of a GLFW window may occur only in the main thread. Do not instantiate a Window in secondary threads.
         */
        Window(const WindowID id, const int width, const int height, const std::string &title,
               GLFWmonitor *monitor = nullptr, const WindowPointer &share = WindowPointer(nullptr));
        
        /**
         *  @brief  The copy constructor is deleted, i.e. a Window can not be copied.
         *  @param  The Window to copy.
         */
        Window(const Window &) = delete;
        
        /**
         *  @brief  The move constructor is deleted, i.e. a Window can not be moved.
         *  @param  The Window to move.
         */
        Window(Window &&) = delete;
        
        /**
         *  @brief  The copy operator is deleted, i.e. a Window can not be copied.
         *  @param  The Window to copy.
         *  @return A reference to this Window.
         */
        Window & operator =(const Window &) = delete;
        
        /**
         *  @brief  The move operator is deleted, i.e. a Window can not be moved.
         *  @param  The Window to move.
         *  @return A reference to this Window.
         */
        Window & operator =(Window &&) = delete;
        
        /**
         *  @brief  Destructor.
         */
        ~Window();
        
        /**
         *  @brief  The destroy method releases all GLFW data owned by this Window. This may only be called from the main thread.
         */
        void destroy();
        
        /**
         *  @brief  returns this Window's assigned ID.
         *  @return this Window's assigned ID.
         */
        WindowID getID() const;
        
        /**
         *  @brief  The bindEventHandler method binds an EventHandler by adding it to the list of handlers in a position determined by the rank r.
         *  @param eh The EventHandler to bind.
         *  @param r  The rank associated to the EventHandler to bind. This determines its position in the list of handlers.
         *  @note   If the handler has been already bound, it is rebound with the eventually new rank. Moreover, an handler may be bound to several windows.
         */
        void bindEventHandler(const EventHandlerPointer &eh, const RankType r);
        
        /**
         *  @brief  The unbindEventHandler methods removes the EventHandler eh from the list of handlers, if present.
         *  @param eh The EventHandler to unbind.
         */
        void unbindEventHandler(const EventHandlerPointer &eh);
        
        /**
         *  @brief  The handleEvent method is called when an event occurs and it just calls the bound handlers in the order given by their rank until
         *          one of them positively handles it.
         *  @param e The event to handle.
         */
        void handleEvent(const EventPointer &e);
        
        /**
         *  @brief  The bindDrawable method binds a Drawable by adding it to the list of drawable objects in a position determined by the rank r.
         *  @param d The Drawable to bind.
         *  @param r The rank associated to the Drawable to bind. This determines its position in the list of drawables.
         *  @note   If the drawable has been already bound, it is rebound with the eventually new rank. Moreover, a drawable may be bound to several windows.
         */
        void bindDrawable(const DrawablePointer &d, const RankType r);
        
        /**
         *  @brief  The unbindDrawable methods removes the Drawable d from the list of drawable objects, if present.
         *  @param d The Drawable to unbind.
         */
        void unbindDrawable(const DrawablePointer &d);
        
        /**
         *  @brief  The draw method is called when this window is rendered and it just calls the bound drawables in the order given by their rank.
         */
        void draw();
        
        /**
         *  @brief  The shouldClose method is a wrapper of glfwWindowShouldClose.
         *  @return true if this window should close, false otherwise.
         */
        bool shouldClose() const;
        
        /**
         *  @brief  The setShouldClose method is a wrapper of glfwSetWindowShouldClose.
         *  @param c true for yes, false for no.
         */
        void setShouldClose(const bool c);
        
        /**
         *  @brief  The setTitle method changes the current title.
         *  @param title The new window title.
         *  @note   This may only be called from the main thread.
         */
        void setTitle(const std::string &title);
        
        /**
         *  @brief  The getPosition method returns the x and y screen coordinate position.
         *  @param x The x coordinate.
         *  @param y The y coordintate.
         *  @note   This may only be called from the main thread.
         */
        void getPosition(int &x, int &y) const;
        
        /**
         *  @brief  The setPosition method sets the x and y screen coordinate position.
         *  @param x The x coordinate.
         *  @param y The y coordintate.
         *  @note   This may only be called from the main thread.
         */
        void setPosition(const int x, const int y);
        
        /**
         *  @brief  The getSize method returns the width and height of this window area.
         *  @param width The width of this window area.
         *  @param height The height of this window area.
         *  @note   This may only be called from the main thread.
         */
        void getSize(int &width, int &height) const;
        
        /**
         *  @brief  The setSize method sets the width and height of this window area.
         *  @param width The width of this window area.
         *  @param height The height of this window area.
         *  @note   This may only be called from the main thread.
         */
        void setSize(const int width, const int height);
        
        /**
         *  @brief  The getFramebufferSize method returns the width and height of this window framebuffer.
         *  @param width The width of this window framebuffer.
         *  @param height The height of this window framebuffer.
         *  @note   This may only be called from the main thread.
         */
        void getFramebufferSize(int &width, int &height) const;
        
        /**
         *  @brief  The getFrameSize method returns size of each of the frame edges of this window.
         *  @param left The left edge size.
         *  @param top The top edge size.
         *  @param right The right edge size.
         *  @param bottom The bottom edge size.
         *  @note   This may only be called from the main thread.
         */
        void getFrameSize(int &left, int &top, int &right, int &bottom) const;
        
        /**
         *    @brief  The getCursorPosition method returns the cursor position, in screen coordinates, relative to
         *            the upper left corner of this window.
         *    @param x The x-coordinate of the cursor.
         *    @param y The y-coordinate of the cursor.
         */
        void getCursorPosition(double &x, double &y) const;
        
        /**
         *    @brief  The setCursorPosition method changes the cursor position, in screen coordinates, relative to
         *            the upper left corner of this window.
         *    @param x The new x-coordinate of the cursor.
         *    @param y The new y-coordinate of the cursor.
         */
        void setCursorPosition(double x, double y);
        
        /**
         *  @brief  The iconify method minimizes this window.
         *  @note   This may only be called from the main thread.
         */
        void iconify();
        
        /**
         *  @brief  The restore method restores this window, if minimized.
         *  @note   This may only be called from the main thread.
         */
        void restore();
        
        /**
         *  @brief  The hide method hides this window, if shown and not fullscreen.
         *  @note   This may only be called from the main thread.
         */
        void hide();
        
        /**
         *  @brief  The show method shows this window, if hidden.
         *  @note   This may only be called from the main thread.
         */
        void show();
        
        /**
         *  @brief  The getMonitor method returns the GLFWMonitor this window uses in fullscreen.
         *  @return This window monitor.
         *  @note   This may only be called from the main thread.
         */
        GLFWmonitor * getMonitor() const;
        
        /**
         *  @brief  The getAttribute method returns this window attributes. See GLFW.
         *  @param  attribute The attribute to read.
         *  @return The value of attribute for this window.
         *  @note   This may only be called from the main thread.
         */
        int getAttribute(const int attribute) const;
        
        /**
         *  @brief  The getUserPointer method returns the user pointer for this window. See GLFW.
         *  @return The user pointer.
         */
        void * getUserPointer() const;
        
        /**
         *  @brief  The setUserPointer method sets the user pointer for this window. See GLFW.
         *  @param pointer The user pointer to set.
         */
        void setUserPointer(void *pointer);
        
        /**
         *  @brief  The swapBuffers method swaps the front and the back buffer.
         */
        void swapBuffers();
        
        /**
         *  @brief  The makeContextCurrent method makes the context associated to this window current.
         */
        void makeContextCurrent();
        
        /**
         *  @brief  The doneCurrentContext method releases the resources held by makeContextCurrent.
         *  @note   A call to this may only follow a call to makeContextCurrent, without nesting, otherwise the behaviour is undefined.
         *          This method does nothing if NO_MULTITHREADING is defined.
         */
        void doneCurrentContext();
        
        
        
    private:
        
        // The WindowManager is friend for letting it access this Window's private data member glfwWindow.
        friend class WindowManager;
        
        /**
         *  @brief  The GLFW window data structure for this Window and its context.
         */
        GLFWwindow                         *glfwWindow;
        
        /**
         *  @brief  The WindowID is the ID of this window, which is assigned at construction time
         *          and remains constant until destruction.
         */
        const WindowID                      windowID;
        
        /**
         *  @brief  The EventHandlerRank struct stores a pointer to an EventHandler and its rank which determines its position in the list.
         */
        using   EventHandlerRank        =   ObjectRank<EventHandlerPointer, RankType>;
        
        /**
         *  @brief  The list of EventHandler pointers sorted by their respective rank, used to determine which one will handle an event first.
         */
        std::deque<EventHandlerRank>        eventHandlers;
        
        /**
         *  @brief  The EventHandlersIterator is an iterator to an element of eventHandlers.
         */
        using   EventHandlersIterator   =   std::deque<EventHandlerRank>::iterator;
        
        /**
         *  @brief  The type of the container used to store and fast retrieve the handler position in the list.
         */
        using   EventHandlerMap         =   std::unordered_map<EventHandlerPointer, EventHandlersIterator>;
        
        /**
         *  @brief  The iterator used to search for elements in the EventHandlerMap.
         */
        using   EventHandlerMapIterator =   EventHandlerMap::iterator;
        
        /**
         *  @brief  The container for storing EventHandler objects currently bound and associated position in list.
         */
        EventHandlerMap                     eventHandlerMap;
        
        /**
         *  @brief  The DrawableRank struct stores a pointer to an Drawable and its rank which determines its position in the list.
         */
        using   DrawableRank            =   ObjectRank<DrawablePointer, RankType>;
        
        /**
         *  @brief  The list of Drawables pointers sorted by their respective rank, used to determine which one will draw first.
         */
        std::deque<DrawableRank>            drawables;
        
        /**
         *  @brief  The DrawablesIterator is an iterator to an element of drawables.
         */
        using   DrawablesIterator       =   std::deque<DrawableRank>::iterator;
        
        /**
         *  @brief  The type of the container used to store and fast retrieve the drawable position in the list.
         */
        using   DrawableMap             =   std::unordered_map<DrawablePointer, DrawablesIterator>;
        
        /**
         *  @brief  The iterator used to search for elements in the DrawableMap.
         */
        using   DrawableMapIterator     =   DrawableMap::iterator;
        
        /**
         *  @brief  The container for storing Drawable objects currently bound and associated position in list.
         */
        DrawableMap                         drawableMap;
        
        
        
#ifndef NO_MULTITHREADING
        /**
         *  @brief  Mutex used to guarantee correct concurrent management of static activities.
         */
        static std::recursive_mutex         globalMutex;
        
        /**
         *  @brief  The type of the ID used to identify a mutex.
         */
        using   MutexID                 =   size_t;
        
        /**
         *  @brief  The ID to identify a mutex. It is actually an index of the mutexes container.
         */
        MutexID                             sharedMutexID;
        
        /**
         *  @brief  The MutexData struct is just a wrapper for a mutex together with its number of users.
         */
        struct MutexData {
            std::recursive_mutex            mutex;
            size_t                          count;
            MutexData() : count(0) {}
        };
        
        /**
         *  @brief  The container of mutexes.
         */
        static std::deque<MutexData>        mutexes;
        
        /**
         *  @brief  The container for reusing freed MutexIDs. It is kept sorted.
         */
        static std::deque<MutexID>          freedMutexes;
        
        /**
         *  @brief  The newMutexID static method books a new or an old & freed ID for mutexes.
         *  @return The booked MutexID.
         */
        static MutexID newMutexID();
        
        /**
         *  @brief  The decreaseMutexCount static method decreases the number of users of a mutex, eventually freeing it.
         *  @param id The ID of the mutex.
         */
        static void decreaseMutexCount(const MutexID id);
#endif
        
        /**
         *  @brief  The WindowMap class is a hash table used to map GLFWWindow pointers to Window IDs.
         */
        using   WindowMap               =   std::unordered_map<GLFWwindow *, WindowID>;
        
        /**
         *  @brief  The iterator for accessing the WindowMap.
         */
        using   WindowMapIterator       =   WindowMap::iterator;
        
        /**
         *  @brief  The map between GLFWWindow pointers and WindowIDs.
         */
        static WindowMap                    windowsMap;
        
        /**
         *  @brief  The container for collecting Windows.
         */
        static std::deque<WindowPointer>    windows;
        
        /**
         *  @brief  The container for reusing freed WindowIDs. It is kept sorted.
         */
        static std::deque<WindowID>         freedWindowIDs;
        
    public:
        /**
         *  @brief  The newWindowID static method books a new or an old & freed ID for windows.
         *  @return The booked WindowID.
         */
        static WindowID newWindowID();
        
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
        static WindowPointer newWindow(const int width, const int height, const std::string &title,
                                       GLFWmonitor *monitor = nullptr, const WindowPointer &share = WindowPointer(nullptr));
        
        /**
         *  @brief  The getWindow static method gives the WindowPointer to a Window given its ID.
         *  @param id The WindowID of the Window to access.
         *  @return The WindowPointer of the requested Window, or a null pointer if it does not exist.
         */
        static WindowPointer getWindow(const WindowID id);
        
        /**
         *  @brief  The getWindowID static method gives the ID of a Window associated to a GLFWWindow object.
         *  @param w The GLFWWindow object pointer.
         *  @return The WindowPointer of the associated Window.
         */
        static WindowID getWindowID(GLFWwindow *w);
        
        /**
         *  @brief  The getALlWindowIDs static method returns the set of all the WindowID currently in use.
         *  @param wIDs The set of WindowIDs.
         */
        static void getAllWindowIDs(std::unordered_set<WindowID> &wIDs);
        
        /**
         *  @brief  The isAnyWindowOpen static method says if there is any Window still open.
         *  @return true if is there any Window still open, false otherwise.
         */
        static bool isAnyWindowOpen();
        
        /**
         *  @brief  The windowsToClose static method returns a set of Windows that have the close flag on.
         *  @param wtc The set of WindowIDs of the Windows to close.
         */
        static void windowsToClose(std::unordered_set<WindowID> &wtc);
        
        /**
         *  @brief  The deleteWindow static method destroys and removes the Window at id.
         *  @param id The ID of the Window to remove.
         */
        static void deleteWindow(const WindowID id);
        
        /**
         *  @brief  The deleteAllWindows static method destroys all current windows and delete everything.
         */
        static void deleteAllWindows();
        
    private:
        /**
         *  @brief  The unmapWindow static method removes the association between a GLFWWindow object and a Window.
         *  @param w The GLFWWindow object to unmap.
         */
        static void unmapWindow(GLFWwindow *w);
        
        /**
         *  @brief  The freeWindowID frees an ID for a later reuse.
         *  @param id The ID to free.
         */
        static void freeWindowID(const WindowID id);
    };
    
}

#endif
