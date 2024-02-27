// Copyright (c) 2015-2024 Giorgio Marcias
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

#include <GLFWM/glfwm.hpp>

namespace glfwm {

#ifndef NO_MULTITHREADING
	std::atomic<double> WindowManager::waitTimeout(std::numeric_limits<double>::infinity());
#else
	double WindowManager::waitTimeout = std::numeric_limits<double>::infinity();
#endif

	/**
	 *    @brief   The init static method initializes GLFW.
	 *    @return true if correctly initialized, false otherwise.
	 */
	bool WindowManager::init() { return glfwInit(); }

	/**
	 *    @brief  The setSwapInterval static method changes the number of screen updates to wait before swapping the
	 * framebuffers (vsync).
	 *    @param intervals How many intervals to wait.
	 */
	void WindowManager::setSwapInterval(const int intervals) { glfwSwapInterval(intervals); }

	/**
	 *  @brief  The resetDefaultHints static method resets all window hints to their default values.
	 *  @note   This may only be called from the main thread.
	 */
	void WindowManager::resetDefaultHints() { glfwDefaultWindowHints(); }

	/**
	 *  @brief  The setHint static method sets a target hint to a given value.
	 *  @param target The target hint.
	 *  @param value  The value to set to the hint.
	 */
	void WindowManager::setHint(const int target, const int value) { glfwWindowHint(target, value); }

	/**
	 *  @brief  The setPoll static method changes the current way of managing the event queue: process any event in the
	 * queue soon, or wait untill any events have occurred and process them.
	 *  @param doPoll true for processing without waiting, false for waiting and then processing.
	 */
	void WindowManager::setPoll(const bool doPoll) {
		if (doPoll)
			setWaitTimeout(0.0);
		else
			setWaitTimeout(std::numeric_limits<double>::infinity());
	}

	/**
	 *  @brief  The setWaitTimeout method changes the current way of managing the event queue: process any event in the
	 * queue soon, or wait untill timeout has expired or any events have occurred and process them.
	 *  @param timeout The maximum time to wait before processing the event queue.
	 *  @note 0 corresponds to poll, infinity corresponds to wait undefinitely, k corresponds to wait k seconds.
	 */
	void WindowManager::setWaitTimeout(const double timeout) {
		waitTimeout = timeout;
		if (waitTimeout == 0.0)
			UpdateMap::notify();
	}

	/**
	 *    @brief The getWaitTimeout method returns the current time to wait in the current polling events management.
	 *    @return The current timeout.
	 */
	double WindowManager::getWaitTimeout() { return waitTimeout; }

	/**
	 *    @brief  The createWindow static method is a convenient way of constructing a new Window and directly
	 * registering the callbacks for the events of type eventTypes.
	 *    @param width      The window width.
	 *    @param height     The window height.
	 *    @param title      The window title.
	 *    @param eventTypes A mask corresponding to the list of event types to register for callbacks.
	 *    @param monitor    A pointer to a monitor for fullscreen, or NULL otherwise.
	 *    @param share      A pointer to another window to share its context with, or NULL otherwise.
	 *    @return A pointer to the new created Window.
	 */
	WindowPointer WindowManager::createWindow(const int width,
	                                          const int height,
	                                          const std::string& title,
	                                          const EventBaseType eventTypes,
	                                          GLFWmonitor* monitor,
	                                          const WindowPointer& share) {
		WindowPointer w = Window::newWindow(width, height, title, monitor, share);
		registerWindowCallbacks(w, eventTypes);
		return w;
	}

	/**
	 *    @brief  The createWindow static method is a convenient way of constructing a new Window and directly
	 * registering the callbacks for the events of type eventTypes.
	 *    @param width      The window width.
	 *    @param height     The window height.
	 *    @param title      The window title.
	 *    @param eventType  A single type of events to register callbacks for.
	 *    @param monitor    A pointer to a monitor for fullscreen, or NULL otherwise.
	 *    @param share      A pointer to another window to share its context with, or NULL otherwise.
	 *    @return A pointer to the new created Window.
	 */
	WindowPointer WindowManager::createWindow(const int width,
	                                          const int height,
	                                          const std::string& title,
	                                          const EventType eventType,
	                                          GLFWmonitor* monitor,
	                                          const WindowPointer& share) {
		return createWindow(width, height, title, static_cast<EventBaseType>(eventType));
	}

	/**
	 *    @brief  The registerWindowCallbacks static method sets callbacks for window to handle events of type
	 * eventTypes.
	 *    @param window     The Window whose events must be handled with the callbacks to register.
	 *    @param eventTypes A mask corresponding to the list of event types to register for callbacks.
	 */
	void WindowManager::registerWindowCallbacks(WindowPointer& window, const EventBaseType eventTypes) {
		if (eventTypes & EventType::WINDOW_POSITION)
			glfwSetWindowPosCallback(window->glfwWindow, windowPositionCallback);
		if (eventTypes & EventType::WINDOW_SIZE)
			glfwSetWindowSizeCallback(window->glfwWindow, windowSizeCallback);
		if (eventTypes & EventType::WINDOW_CLOSE)
			glfwSetWindowCloseCallback(window->glfwWindow, windowCloseCallback);
		if (eventTypes & EventType::WINDOW_REFRESH)
			glfwSetWindowRefreshCallback(window->glfwWindow, windowRefreshCallback);
		if (eventTypes & EventType::WINDOW_FOCUS)
			glfwSetWindowFocusCallback(window->glfwWindow, windowFocusCallback);
		if (eventTypes & EventType::WINDOW_MAXIMIZE)
			glfwSetWindowMaximizeCallback(window->glfwWindow, windowMaximizeCallback);
		if (eventTypes & EventType::WINDOW_ICONIFY)
			glfwSetWindowIconifyCallback(window->glfwWindow, windowIconifyCallback);
		if (eventTypes & EventType::FRAMEBUFFERSIZE)
			glfwSetFramebufferSizeCallback(window->glfwWindow, windowFramebufferSizeCallback);
		if (eventTypes & EventType::CONTENTSCALE)
			glfwSetWindowContentScaleCallback(window->glfwWindow, windowContentScaleCallback);
		if (eventTypes & EventType::CHAR)
			glfwSetCharCallback(window->glfwWindow, inputCharCallback);
		if (eventTypes & EventType::CHARMOD)
			glfwSetCharModsCallback(window->glfwWindow, inputCharModCallback);
		if (eventTypes & EventType::CURSOR_ENTER)
			glfwSetCursorEnterCallback(window->glfwWindow, inputCursorEnterCallback);
		if (eventTypes & EventType::CURSOR_POSITION)
			glfwSetCursorPosCallback(window->glfwWindow, inputCursorPostionCallback);
		if (eventTypes & EventType::DROP)
			glfwSetDropCallback(window->glfwWindow, inputDropCallback);
		if (eventTypes & EventType::KEY)
			glfwSetKeyCallback(window->glfwWindow, inputKeyCallback);
		if (eventTypes & EventType::MOUSE_BUTTON)
			glfwSetMouseButtonCallback(window->glfwWindow, inputMouseButtonCallback);
		if (eventTypes & EventType::SCROLL)
			glfwSetScrollCallback(window->glfwWindow, inputScrollCallback);
	}

	/**
	 *    @brief  The registerWindowCallbacks static method sets callbacks for window to handle events of type
	 * eventType.
	 *    @param window     The Window whose events must be handled with the callbacks to register.
	 *    @param eventTypes A single type of events to register callbacks for.
	 */
	void WindowManager::registerWindowCallbacks(WindowPointer& window, const EventType eventType) {
		registerWindowCallbacks(window, static_cast<EventBaseType>(eventType));
	}

	/**
	 *    @brief  The getCurrentContext static method returns the Window whose context is the current one.
	 */
	WindowPointer WindowManager::getCurrentContext() {
		return Window::getWindow(Window::getWindowID(glfwGetCurrentContext()));
	}

	/**
	 *    @brief  The mainLoop static method executes the loop of event handling and window rendering.
	 *    @note   The way of managing the event queue can be changed even while this method is running.
	 */
	void WindowManager::mainLoop() {
		WindowGroupID gID;
		WindowGroupPointer g;
		WindowPointer w;
		std::unordered_set<WindowGroupID> gIDs;
		std::unordered_set<WindowID> wIDs;

		// ensure first rendering
		UpdateMap::setToUpdate(AllWindowGroupIDs, AllWindowIDs);

		// do loop
		do {
			// update groups and windows
			while (!UpdateMap::empty()) {
				UpdateMap::popGroup(gID, wIDs);
				if (gID == AllWindowGroupIDs) {
					WindowGroup::getAllWindowGroupIDs(gIDs);
					for (auto id : gIDs) {
						g = WindowGroup::getGroup(id);
						if (g) {
							g->setWindowToUpdate(WholeGroupWindowIDs);
							g->process();
						}
					}
					WindowGroup::getAllUngroupedWindowIDs(wIDs);
					for (auto id : wIDs) {
						w = Window::getWindow(id);
						if (w) {
							w->makeContextCurrent();
							w->draw();
							w->swapBuffers();
							w->doneCurrentContext();
						}
					}
				} else {
					g = WindowGroup::getGroup(gID);
					if (g) {
						for (auto& id : wIDs)
							g->setWindowToUpdate(id);
						g->process();
					} else {
						gIDs.clear();
						for (auto& id : wIDs) {
							g = WindowGroup::getGroup(WindowGroup::getWindowGroup(id));
							if (g) {
								g->setWindowToUpdate(id);
								gIDs.insert(g->getID());
							} else {
								w = Window::getWindow(id);
								if (w) {
									w->makeContextCurrent();
									w->draw();
									w->swapBuffers();
									w->doneCurrentContext();
								}
							}
						}
						for (auto id : gIDs) {
							g = WindowGroup::getGroup(id);
							if (g)
								g->process();
						}
					}
				}
			}

			// manage events
			if (waitTimeout == 0.0) {
				glfwPollEvents();
				UpdateMap::setToUpdate(AllWindowGroupIDs, AllWindowIDs);
			} else if (waitTimeout == std::numeric_limits<double>::infinity()) {
				glfwWaitEvents();
			} else {
				glfwWaitEventsTimeout(waitTimeout);
			}

			// check for windows to close (and detach from groups)
			Window::windowsToClose(wIDs);
			for (auto id : wIDs) {
				g = WindowGroup::getGroup(WindowGroup::getWindowGroup(id));
				if (g)
					g->detachWindow(id);
				Window::deleteWindow(id);
			}

		} while (Window::isAnyWindowOpen());
	}

	/**
	 *    @brief  The terminate static method deletes all the remained Windows and WindowGroups and then terminate GLFW.
	 *    @note   Call this method after mainLoop.
	 */
	void WindowManager::terminate() {
		WindowGroup::deleteAllWindowGroups();
		Window::deleteAllWindows();
		glfwTerminate();
	}

	// callbacks

	void WindowManager::windowPositionCallback(GLFWwindow* glfwWindow, int x, int y) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Position event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ewp = std::make_shared<EventWindowPosition>(wID, x, y);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ewp);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::windowSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Size event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ews = std::make_shared<EventWindowSize>(wID, width, height);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ews);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::windowCloseCallback(GLFWwindow* glfwWindow) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Close event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ewc = std::make_shared<EventWindowClose>(wID);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ewc);
			w->doneCurrentContext();
			// do not update the window, as any handled here may have deallocated any resources
		}
	}

	void WindowManager::windowRefreshCallback(GLFWwindow* glfwWindow) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Refresh event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ewr = std::make_shared<EventWindowRefresh>(wID);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ewr);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::windowFocusCallback(GLFWwindow* glfwWindow, int hasFocus) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Focus event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ewf = std::make_shared<EventWindowFocus>(wID, hasFocus == GL_TRUE);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ewf);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::windowMaximizeCallback(GLFWwindow* glfwWindow, int toMaximize) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Maximize event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ewi = std::make_shared<EventWindowMaximize>(wID, toMaximize == GL_TRUE);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ewi);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::windowIconifyCallback(GLFWwindow* glfwWindow, int toIconify) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Iconify event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ewi = std::make_shared<EventWindowIconify>(wID, toIconify == GL_TRUE);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ewi);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::windowFramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Framebugger size event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer efs = std::make_shared<EventFrameBufferSize>(wID, width, height);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(efs);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::windowContentScaleCallback(GLFWwindow* glfwWindow, float xScale, float yScale) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Content scale event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ecs = std::make_shared<EventContentScale>(wID, xScale, yScale);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ecs);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::inputMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Mouse button event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer emb = std::make_shared<EventMouseButton>(
		    wID, static_cast<MouseButtonType>(button), static_cast<ActionType>(action), mods);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(emb);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::inputCursorPostionCallback(GLFWwindow* glfwWindow, double x, double y) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Cursor position event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ecp = std::make_shared<EventCursorPosition>(wID, x, y);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ecp);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::inputCursorEnterCallback(GLFWwindow* glfwWindow, int enter) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Cursor enter event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ece = std::make_shared<EventCursorEnter>(wID, enter == GL_TRUE);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ece);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::inputScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Scroll event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer es = std::make_shared<EventScroll>(wID, xOffset, yOffset);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(es);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::inputKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Input key event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ek = std::make_shared<EventKey>(
		    wID, static_cast<KeyType>(key), static_cast<char32_t>(scancode), static_cast<ActionType>(action), mods);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ek);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::inputCharCallback(GLFWwindow* glfwWindow, unsigned int codepoint) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Input char event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ec = std::make_shared<EventChar>(wID, static_cast<char32_t>(codepoint));
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ec);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::inputCharModCallback(GLFWwindow* glfwWindow, unsigned int codepoint, int mods) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Input char mod event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		// if found, make it handle the event
		EventPointer ecm = std::make_shared<EventCharMod>(wID, static_cast<char32_t>(codepoint), mods);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ecm);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

	void WindowManager::inputDropCallback(GLFWwindow* glfwWindow, int count, const char** paths) {
		// find the target Window
		WindowID wID = Window::getWindowID(glfwWindow);
		if (wID > LastWindowID) {
			std::cout << "Warning. Drop event received for unregistered Window. Discarded." << std::endl;
			return;
		}
		if (count <= 0)
			return;
		// if found, make it handle the event
		std::list<std::string> pathStrings;
		for (int i = 0; i < count; ++i)
			pathStrings.push_back(paths[i]);
		EventPointer ed = std::make_shared<EventDrop>(wID, pathStrings);
		WindowPointer w = Window::getWindow(wID);
		if (w) {
			w->makeContextCurrent();
			w->handleEvent(ed);
			w->doneCurrentContext();
			// if this window is being rendered concurrently, update soon
			WindowGroupID gID = WindowGroup::getWindowGroup(wID);
#ifndef NO_MULTITHREADING
			WindowGroupPointer g = WindowGroup::getGroup(gID);
			if (g && g->isRunningConcurrently()) {
				g->setWindowToUpdate(wID);
				g->process();
			} else { // otherwise update all together
				UpdateMap::setToUpdate(gID, wID);
			}
#else
			UpdateMap::setToUpdate(gID, wID);
#endif
		}
	}

}
