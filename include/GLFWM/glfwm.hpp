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

#ifndef GLFWM_HPP
#define GLFWM_HPP

#include <GLFWM/window_group.hpp>

namespace glfwm {

	/// The WindowManager class provides static methods for managing Windows, Groups, Events and the loop.
	class WindowManager {
	  public:
		/**
		 *    @brief   The init static method initializes GLFW.
		 *    @return true if correctly initialized, false otherwise.
		 */
		static bool init();

		/**
		 *    @brief  The setSwapInterval static method changes the number of screen updates to wait before swapping the
		 * framebuffers (vsync).
		 *    @param intervals How many intervals to wait.
		 */
		static void setSwapInterval(const int intervals);

		/**
		 *  @brief  The resetDefaultHints static method resets all window hints to their default values.
		 *  @note   This may only be called from the main thread.
		 */
		static void resetDefaultHints();

		/**
		 *  @brief  The setHint static method sets a target hint to a given value.
		 *  @param target The target hint.
		 *  @param value  The value to set to the hint.
		 */
		static void setHint(const int target, const int value);

		/**
		 *  @brief  The setPoll static method changes the current way of managing the event queue: process any event in
		 * the queue soon, or wait untill any events have occurred and process them.
		 *  @param doPoll true for processing without waiting, false for waiting and then processing.
		 */
		static void setPoll(const bool doPoll);

		/**
		 *  @brief  The setWaitTimeout method changes the current way of managing the event queue: process any event in
		 * the queue soon, or wait untill timeout has expired or any events have occurred and process them.
		 *  @param timeout The maximum time to wait before processing the event queue.
		 *  @note 0 corresponds to poll, infinity corresponds to wait undefinitely, k corresponds to wait k seconds.
		 */
		static void setWaitTimeout(const double timeout = std::numeric_limits<double>::infinity());

		/**
		 *    @brief The getWaitTimeout method returns the current time to wait in the current polling events
		 * management.
		 *    @return The current timeout.
		 */
		static double getWaitTimeout();

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
		static WindowPointer createWindow(const int width,
		                                  const int height,
		                                  const std::string& title,
		                                  const EventBaseType eventTypes = allEventTypes,
		                                  GLFWmonitor* monitor = nullptr,
		                                  const WindowPointer& share = WindowPointer(nullptr));

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
		static WindowPointer createWindow(const int width,
		                                  const int height,
		                                  const std::string& title,
		                                  const EventType eventType,
		                                  GLFWmonitor* monitor = nullptr,
		                                  const WindowPointer& share = WindowPointer(nullptr));

		/**
		 *    @brief  The registerWindowCallbacks static method sets callbacks for window to handle events of type
		 * eventTypes.
		 *    @param window     The Window whose events must be handled with the callbacks to register.
		 *    @param eventTypes A mask corresponding to the list of event types to register for callbacks.
		 */
		static void registerWindowCallbacks(WindowPointer& window, const EventBaseType eventTypes = allEventTypes);

		/**
		 *    @brief  The registerWindowCallbacks static method sets callbacks for window to handle events of type
		 * eventType.
		 *    @param window     The Window whose events must be handled with the callbacks to register.
		 *    @param eventTypes A single type of events to register callbacks for.
		 */
		static void registerWindowCallbacks(WindowPointer& window, const EventType eventType);

		/**
		 *    @brief  The getCurrentContext static method returns the Window whose context is the current one.
		 */
		static WindowPointer getCurrentContext();

		/**
		 *    @brief  The mainLoop static method executes the loop of event handling and window rendering.
		 *    @note   The way of managing the event queue can be changed even while this method is running.
		 */
		static void mainLoop();

		/**
		 *    @brief  The terminate static method deletes all the remained Windows and WindowGroups and then terminate
		 * GLFW.
		 *    @note   Call this method after mainLoop.
		 */
		static void terminate();

	  private:
		// callbacks
		static void windowPositionCallback(GLFWwindow* glfwWindow, int x, int y);
		static void windowSizeCallback(GLFWwindow* glfwWindow, int width, int height);
		static void windowCloseCallback(GLFWwindow* glfwWindow);
		static void windowRefreshCallback(GLFWwindow* glfwWindow);
		static void windowFocusCallback(GLFWwindow* glfwWindow, int hasFocus);
		static void windowMaximizeCallback(GLFWwindow* glfwWindow, int toMxamize);
		static void windowIconifyCallback(GLFWwindow* glfwWindow, int toIconify);
		static void windowFramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height);
		static void windowContentScaleCallback(GLFWwindow* glfwWindow, float xScale, float yScale);
		static void inputMouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
		static void inputCursorPostionCallback(GLFWwindow* glfwWindow, double x, double y);
		static void inputCursorEnterCallback(GLFWwindow* glfwWindow, int enter);
		static void inputScrollCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset);
		static void inputKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
		static void inputCharCallback(GLFWwindow* glfwWindow, unsigned int codepoint);
		static void inputCharModCallback(GLFWwindow* glfwWindow, unsigned int codepoint, int mods);
		static void inputDropCallback(GLFWwindow* glfwWindow, int count, const char** paths);

#ifndef NO_MULTITHREADING
		static std::atomic<double> waitTimeout; ///< Timeout for the polling event management: 0 -> poll, inf -> wait
		                                        ///< indefinitely, k -> wait k seconds.
#else
		static double waitTimeout; ///< Timeout for the polling event management: 0 -> poll, inf -> wait indefinitely, k
		                           ///< -> wait k seconds.
#endif
	};

}

#endif
