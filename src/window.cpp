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

#include <GLFWM/window.hpp>

namespace glfwm {

	/**
	 *  @brief  Contructor. This can be called only in the main thread.
	 *  @param  id The ID of this new window.
	 *  @param  width The width of the window area.
	 *  @param  height The height of the window area.
	 *  @param  title The title of the window.
	 *  @param  monitor The GLFWMonitor to associate this window to in case of fullscreen. Use nullptr otherwise.
	 *  @param  share Another Window to share the context with. Use nullptr for a non-shared context.
	 *  @note   The creation of a GLFW window may occur only in the main thread. Do not instantiate a Window in
	 * secondary threads.
	 */
	Window::Window(const WindowID id,
	               const int width,
	               const int height,
	               const std::string& title,
	               GLFWmonitor* monitor,
	               const WindowPointer& share)
	    : windowID(id)
#ifndef NO_MULTITHREADING
	      ,
	      sharedMutexID(share ? share->sharedMutexID : newMutexID())
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
	Window::~Window() { destroy(); }

	/**
	 *  @brief  The destroy method releases all GLFW data owned by this Window. This may only be called from the main
	 * thread.
	 */
	void Window::destroy() {
#ifndef NO_MULTITHREADING
		if (sharedMutexID >= mutexes.size())
			return; // probably this Window has been already destroied at this moment
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
	WindowID Window::getID() const { return windowID; }

	/**
	 *  @brief  The bindEventHandler method binds an EventHandler by adding it to the list of handlers in a position
	 * determined by the rank r.
	 *  @param eh The EventHandler to bind.
	 *  @param r  The rank associated to the EventHandler to bind. This determines its position in the list of handlers.
	 *  @note   If the handler has been already bound, it is rebound with the eventually new rank.
	 */
	void Window::bindEventHandler(const EventHandlerPointer& eh, const RankType r) {
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
	void Window::unbindEventHandler(const EventHandlerPointer& eh) {
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
	 *  @brief  The handleEvent method is called when an event occurs and it just calls the bound handlers in the order
	 * given by their rank until one of them positively handles it.
	 *  @param e The event to handle.
	 */
	void Window::handleEvent(const EventPointer& e) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif

		// check if this is the right recipient
		if (e->getWindowID() != windowID)
			return;

		// search the first handler that handles event e
		for (auto& h : eventHandlers)
			if (h.object->getHandledEventTypes() & e->getEventType() && h.object->handle(e))
				return;
	}

	/**
	 *  @brief  The bindDrawable method binds a Drawable by adding it to the list of drawable objects in a position
	 * determined by the rank r.
	 *  @param d The Drawable to bind.
	 *  @param r  The rank associated to the Drawable to bind. This determines its position in the list of drawables.
	 *  @note   If the drawable has been already bound, it is rebound with the eventually new rank. Moreover, a drawable
	 * may be bound to several windows.
	 */
	void Window::bindDrawable(const DrawablePointer& d, const RankType r) {
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
	void Window::unbindDrawable(const DrawablePointer& d) {
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
	 *  @brief  The draw method is called when this window is rendered and it just calls the bound drawables in the
	 * order given by their rank.
	 */
	void Window::draw() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		// draw each drawable in sequence
		for (auto& d : drawables)
			d.object->draw(windowID);
	}

	/**
	 *  @brief  The shouldClose method is a wrapper of glfwWindowShouldClose.
	 *  @return true if this window should close, false otherwise.
	 */
	bool Window::shouldClose() const {
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
	 *  @brief  The getTitle method returns the current title.
	 *  @note   This may only be called from the main thread.
	 *  @return This window's current title.
	 */
	std::string Window::getTitle() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			return glfwGetWindowTitle(glfwWindow);
		return std::string();
	}

	/**
	 *  @brief  The setTitle method changes the current title.
	 *  @param title The new window title.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::setTitle(const std::string& title) {
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
	void Window::getPosition(int& x, int& y) const {
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
	void Window::setPosition(const int x, const int y) {
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
	void Window::getSize(int& width, int& height) const {
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
	void Window::setSize(const int width, const int height) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetWindowSize(glfwWindow, width, height);
	}

	/**
	 *  @brief  The setSizeLimits method sets the size limits of the client area of the specified window.
	 *
	 *  If the window is full screen, the size limits only take effect once it is made windowed.
	 *  If the window is not resizable, this function does nothing.
	 *  The size limits are applied immediately to a windowed mode window and may cause it to be resized.
	 *  The maximum dimensions must be greater than or equal to the minimum dimensions and all must be
	 *  greater than or equal to zero.
	 *
	 *  @param minWidth  The minimum width, in screen coordinates, of the client area, or GLFW_DONT_CARE.
	 *  @param minHeight The minimum height, in screen coordinates, of the client area, or GLFW_DONT_CARE.
	 *  @param maxWidth  The maximum width, in screen coordinates, of the client area, or GLFW_DONT_CARE.
	 *  @param maxHeight The maximum height, in screen coordinates, of the client area, or GLFW_DONT_CARE.
	 *  @note   If you set size limits and an aspect ratio that conflict, the results are undefined.
	 *          This may only be called from the main thread.
	 */
	void Window::setSizeLimits(const int minWidth, const int minHeight, const int maxWidth, const int maxHeight) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetWindowSizeLimits(glfwWindow, minWidth, minHeight, maxWidth, maxHeight);
	}

	/**
	 *  @brief  The setAspectRatio method sets the required aspect ratio of the client area of the specified window.
	 *
	 *  If the window is full screen, the aspect ratio only takes effect once it is made windowed.
	 *  If the window is not resizable, this function does nothing.
	 *  The aspect ratio is specified as a numerator and a denominator and both values must be greater than zero.
	 *  For example, the common 16:9 aspect ratio is specified as 16 and 9, respectively.
	 *  If the numerator and denominator is set to GLFW_DONT_CARE then the aspect ratio limit is disabled.
	 *  The aspect ratio is applied immediately to a windowed mode window and may cause it to be resized.
	 *
	 *  @param numerator   The numerator of the desired aspect ratio, or GLFW_DONT_CARE.
	 *  @param denominator The denominator of the desired aspect ratio, or GLFW_DONT_CARE.
	 *  @note   If you set size limits and an aspect ratio that conflict, the results are undefined.
	 *          This may only be called from the main thread.
	 */
	void Window::setAspectRatio(const int numerator, const int denominator) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetWindowAspectRatio(glfwWindow, numerator, denominator);
	}

	/**
	 *  @brief  The getFramebufferSize method returns the width and height of this window framebuffer.
	 *  @param width The width of this window framebuffer.
	 *  @param height The height of this window framebuffer.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::getFramebufferSize(int& width, int& height) const {
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
	void Window::getFrameSize(int& left, int& top, int& right, int& bottom) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwGetWindowFrameSize(glfwWindow, &left, &top, &right, &bottom);
	}

	/**
	 *  @brief  The getContentScale method retrieves the content scale for the specified window.
	 *
	 *  The content scale is the ratio between the current DPI and the platform's default DPI. This is especially
	 *  important for text and any UI elements. If the pixel dimensions of your UI scaled by this look appropriate
	 *  on your machine then it should appear at a reasonable size on other machines regardless of their DPI and
	 *  scaling settings. This relies on the system DPI and scaling settings being somewhat correct.
	 *
	 *  On systems where each monitors can have its own content scale, the window content scale will depend on
	 *  which monitor the system considers the window to be on.
	 *  @param xScale The scale in x of this window content.
	 *  @param yScale The scale in y of this window content.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::getContentScale(float& xScale, float& yScale) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwGetWindowContentScale(glfwWindow, &xScale, &yScale);
	}

	/**
	 * @brief The getOpacity method returns the opacity of the window, including any decorations.
	 *
	 * The opacity (or alpha) value is a positive finite number between zero and one, where zero is fully
	 * transparent and one is fully opaque. If the system does not support whole window transparency, this
	 * method always returns one. The initial opacity value for newly created windows is one.
	 *
	 * @note   This may only be called from the main thread.
	 * @return float The opacity value of this window.
	 */
	float Window::getOpacity() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			return glfwGetWindowOpacity(glfwWindow);
		return 0;
	}

	/**
	 * @brief The setOpacity method sets the opacity of the window, including any decorations.
	 *
	 * The opacity (or alpha) value is a positive finite number between zero and one, where zero is fully
	 * transparent and one is fully opaque. The initial opacity value for newly created windows is one.
	 * A window created with framebuffer transparency may not use whole window transparency. The results of doing
	 * this are undefined.
	 *
	 * @param opacity The opacity to set to this window.
	 * @note   This may only be called from the main thread.
	 */
	void Window::setOpacity(float opacity) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetWindowOpacity(glfwWindow, opacity);
	}

	/**
	 * @brief The requestAttention method requests user attention to the specified window.
	 *
	 * On platforms where this is not supported, attention is requested to the application as a whole.
	 * Once the user has given attention, usually by focusing the window or application, the system will
	 * end the request automatically.
	 *
	 * Note that, on macOs attention is requested to the application as a whole, not the specific window.
	 *
	 * @note   This may only be called from the main thread.
	 */
	void Window::requestAttention() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwRequestWindowAttention(glfwWindow);
	}

	/**
	 *  @brief  The getInputMode method returns the current value of an input mode for this Window.
	 *  @param inputMode The input mode to query for its value.
	 *  @return The value of the specified input mode.
	 *  @note   This may only be called from the main thread.
	 */
	InputModeValueType Window::getInputMode(const InputModeType inputMode) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			return static_cast<InputModeValueType>(
			    glfwGetInputMode(glfwWindow, static_cast<InputModeBaseType>(inputMode)));
		else if (inputMode == InputModeType::CURSOR)
			return InputModeValueType::CURSOR_NORMAL;
		else
			return InputModeValueType::STICKY_FALSE;
	}

	/**
	 *  @brief  The setInputMode method sets a new value for the specified input mode.
	 *  @param inputMode      The input mode whose value must be changed.
	 *  @param inputModeValue The value for the specified input mode.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::setInputMode(const InputModeType inputMode, const InputModeValueType inputModeValue) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetInputMode(glfwWindow,
			                 static_cast<InputModeBaseType>(inputMode),
			                 static_cast<InputModeValueBaseType>(inputModeValue));
	}

	/**
	 *  @brief  The getKey method returns the last state reported for the specified key.
	 *  @param key The key to query for its last state.
	 *  @return The last state of the specified key.
	 *  @note   This may only be called from the main thread.
	 */
	ActionType Window::getKey(const KeyType key) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			return static_cast<ActionType>(glfwGetKey(glfwWindow, static_cast<KeyBaseType>(key)));
		else
			return ActionType::NONE;
	}

	/**
	 *  @brief  The getMouseButton method returns the last state reported for the specified mouse button.
	 *  @param mouseButton  The mouse button to query for its last state.
	 *  @return The last state of the specified mouse button.
	 *  @note   This may only be called from the main thread.
	 */
	ActionType Window::getMouseButton(const MouseButtonType mouseButton) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			return static_cast<ActionType>(
			    glfwGetMouseButton(glfwWindow, static_cast<MouseButtonBaseType>(mouseButton)));
		else
			return ActionType::NONE;
	}

	/**
	 *  @brief  The setCursor method sets the cursor shape to be used when the cursor is over this Window area.
	 *  @param cursor The cursor to set. If a null pointer is passed, the default arrow cursor is set.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::setCursor(GLFWcursor* cursor) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetCursor(glfwWindow, cursor);
	}

	/**
	 *  @brief  The getCursorPosition method returns the cursor position, in screen coordinates, relative to
	 *          the upper left corner of this window.
	 *  @param x The x-coordinate of the cursor.
	 *  @param y The y-coordinate of the cursor.
	 */
	void Window::getCursorPosition(double& x, double& y) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwGetCursorPos(glfwWindow, &x, &y);
	}

	/**
	 *  @brief  The setCursorPosition method changes the cursor position, in screen coordinates, relative to
	 *          the upper left corner of this window.
	 *  @param x The new x-coordinate of the cursor.
	 *  @param y The new y-coordinate of the cursor.
	 */
	void Window::setCursorPosition(double x, double y) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetCursorPos(glfwWindow, x, y);
	}

	/**
	 *  @brief  The setIcon method sets the icon of this window.
	 *
	 *  If passed an array of candidate images, those of or closest to the sizes desired by the system are selected.
	 *  If no images are specified, the window reverts to its default icon.
	 *  The desired image sizes varies depending on platform and system settings. The selected images will be rescaled
	 *  as needed. Good sizes include 16x16, 32x32 and 48x48.
	 *
	 *  @param count  The number of images in the specified array, or zero to revert to the default window icon.
	 *  @param images The images to create the icon from. This is ignored if count is zero.
	 *  @note   On Mac OS X, the window has no icon, as it is not a document window, so this function does nothing.
	 *          The dock icon will be the same as the application bundle's icon.
	 *          This may only be called from the main thread.
	 */
	void Window::setIcon(const int count, const GLFWimage* images) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetWindowIcon(glfwWindow, count, images);
	}

	/**
	 *  @brief  The maximize method maximizes the specified window if it was previously not maximized.
	 *          If the window is already maximized, this function does nothing.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::maximize() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwMaximizeWindow(glfwWindow);
	}

	/**
	 *  @brief  The iconify method minimizes this window.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::iconify() {
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
	void Window::restore() {
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
	void Window::hide() {
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
	void Window::show() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwShowWindow(glfwWindow);
	}

	/**
	 *  @brief  The focus method brings the specified window to front and sets input focus. By default,
	 *          both windowed and full screen mode windows are focused when initially created. Set the GLFW_FOCUSED to
	 * disable this behavior.
	 *  @note   The window should already be visible and not iconified. Do not use this function to steal focus
	 *          from other applications unless you are certain that is what the user wants.
	 *          Focus stealing can be extremely disruptive.
	 *          This may only be called from the main thread.
	 */
	void Window::focus() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwFocusWindow(glfwWindow);
	}

	/**
	 *  @brief  The getMonitor method returns the GLFWMonitor this window uses in fullscreen.
	 *  @return This window monitor.
	 */
	GLFWmonitor* Window::getMonitor() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			return glfwGetWindowMonitor(glfwWindow);
		return nullptr;
	}

	/**
	 *  @brief  The setMonitor method sets this window at full-screen (if monitor != nullptr) or windowed (otherwise).
	 *
	 *  When setting a monitor, this function updates the width, height and refresh rate of the desired video mode
	 *  and switches to the video mode closest to it.
	 *  The window position is ignored when setting a monitor.
	 *  When the monitor is nullptr, the position, width and height are used to place the window client area.
	 *  The refresh rate is ignored when no monitor is specified.
	 *  If you only wish to update the resolution of a full screen window or the size of a windowed mode window, see
	 * setSize. When a window transitions from full screen to windowed mode, this function restores any previous window
	 * settings such as whether it is decorated, floating, resizable, has size or aspect ratio limits, etc..
	 *
	 *  @param monitor      The desired monitor, or nullptr to set windowed mode.
	 *  @param xpos         The desired x-coordinate of the upper-left corner of the client area.
	 *  @param ypos         The desired y-coordinate of the upper-left corner of the client area.
	 *  @param width        The desired with, in screen coordinates, of the client area or video mode.
	 *  @param height       The desired height, in screen coordinates, of the client area or video mode.
	 *  @param refreshRate  The desired refresh rate, in Hz, of the video mode, or GLFW_DONT_CARE.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::setMonitor(GLFWmonitor* monitor,
	                        const int xpos,
	                        const int ypos,
	                        const int width,
	                        const int height,
	                        const int refreshRate) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetWindowMonitor(glfwWindow, monitor, xpos, ypos, width, height, refreshRate);
	}

	/**
	 *  @brief  The getAttribute method returns this window attributes. See GLFW.
	 *  @param  attribute The attribute to read.
	 *  @return The value of attribute for this window.
	 *  @note   This may only be called from the main thread.
	 */
	int Window::getAttribute(const int attribute) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			return glfwGetWindowAttrib(glfwWindow, attribute);
		return 0;
	}

	/**
	 *  @brief  The setAttribute method sets this window `attribute` to the specified `value`. See GLFW.
	 *  @param  attribute The attribute to write.
	 *  @param  The value of attribute to set for this window.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::setAttribute(const int attribute, const int value) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			return glfwSetWindowAttrib(glfwWindow, attribute, value);
	}

	/**
	 *  @brief  The getUserPointer method returns the user pointer for this window. See GLFW.
	 *  @return The user pointer.
	 */
	void* Window::getUserPointer() const {
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
	void Window::setUserPointer(void* pointer) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetWindowUserPointer(glfwWindow, pointer);
	}

	/**
	 *  @brief  The getClipboardString method returns the system clipboard text, if any.
	 *  @param text The output system clipboard text.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::getClipboardString(std::string& text) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		text.clear();
		if (glfwWindow) {
			const char* t = glfwGetClipboardString(glfwWindow);
			if (t)
				text = t;
		}
	}

	/**
	 *  @brief  The setClipboardString method sets the system clipboard text.
	 *  @param text The system clipboard text to set.
	 *  @note   This may only be called from the main thread.
	 */
	void Window::setClipboardString(const std::string& text) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (glfwWindow)
			glfwSetClipboardString(glfwWindow, text.c_str());
	}

	/**
	 *  @brief  The swapBuffers method swaps the front and the back buffer.
	 */
	void Window::swapBuffers() {
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
	void Window::makeContextCurrent() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		mutexes[sharedMutexID].mutex.lock();
#endif
		if (glfwWindow)
			glfwMakeContextCurrent(glfwWindow);
	}

	/**
	 *  @brief  The doneCurrentContext method releases the resources held by makeContextCurrent.
	 *  @note   A call to this may only follow a call to makeContextCurrent, without nesting, otherwise the behaviour is
	 * undefined. This method does nothing if NO_MULTITHREADING is defined.
	 */
	void Window::doneCurrentContext() {
#ifndef NO_MULTITHREADING
		mutexes[sharedMutexID].mutex.unlock();
#endif
	}

#ifdef VK_VERSION_1_0
	/**
	 *  @brief The createVulkanWindowSurface method creates a Vulkan surface for this window.
	 *
	 *  If the Vulkan loader was not found at initialization, this function returns VK_ERROR_INITIALIZATION_FAILED
	 *  and generates a GLFW_API_UNAVAILABLE error. Call glfwVulkanSupported to check whether the Vulkan loader was
	 * found. If the required window surface creation instance extensions are not available or if the specified instance
	 * was not created with these extensions enabled, this function returns VK_ERROR_EXTENSION_NOT_PRESENT and generates
	 * a GLFW_API_UNAVAILABLE error. Call glfwGetRequiredInstanceExtensions to check what instance extensions are
	 * required. The window surface must be destroyed before the specified Vulkan instance. It is the responsibility of
	 * the caller to destroy the window surface. GLFW(M) does not destroy it for you. Call vkDestroySurfaceKHR to
	 * destroy the surface.
	 *
	 *    @param instance  The Vulkan instance to create the surface in.
	 *    @param allocator The allocator to use, or null_ptr to use the default allocator.
	 *    @param surface   Where to store the handle of the surface. This is set to VK_NULL_HANDLE if an error occurred.
	 *    @return VK_SUCCESS if successful, or a Vulkan error code if an error occurred.
	 */
	VkResult Window::createVulkanWindowSurface(VkInstance instance,
	                                           const VkAllocationCallbacks* allocator,
	                                           VkSurfaceKHR* surface) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			throw std::runtime_error(
			    std::string("Error. GLFW window does not exist. Impossible to create Vulkan surface."));
		return glfwCreateWindowSurface(instance, glfwWindow, allocator, surface);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_WIN32
	/**
	 *  @brief Get the Win32 handle associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The Win32 handle associated to this Window.
	 */
	HWND Window::getWin32Window() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return NULL;
		return glfwGetWin32Window(glfwWindow);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_COCOA
	/**
	 *  @brief Get the NSWindow associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The NSWindow associated to this Window.
	 */
	id Window::getCocoaWindow() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return nil;
		return glfwGetCocoaWindow(glfwWindow);
	}

	/**
	 *  @brief Get the NSView id associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The NSView id associated to this Window.
	 */
	id Window::getCocoaView() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return nil;
		return glfwGetCocoaView(glfwWindow);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_X11
	/**
	 *  @brief Get the X11's Window associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The X11's Window associated to this Window.
	 */
	::Window Window::getX11Widow() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return None;
		return glfwGetX11Window(glfwWindow);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_WAYLAND
	/**
	 *  @brief Get the Wayland's wl_surface associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The Wayland's wl_surface associated to this Window.
	 */
	struct wl_surface* Window::getWaylandWidow() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return nullptr;
		return glfwGetWaylandWindow(glfwWindow);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_WGL
	/**
	 *  @brief Get the WGL's context associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The WGL's context associated to this Window.
	 */
	HGLRC Window::getWGLContext() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return NULL;
		return glfwGetWGLContext(glfwWindow);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_NSGL
	/**
	 *  @brief Get the NSOpenGLContext associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The NSOpenGLContext associated to this Window.
	 */
	id Window::getNSGLContext() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return nil;
		return glfwGetNSGLContext(glfwWindow);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_GLX
	/**
	 *  @brief Get the GLX's context associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The GLX's context associated to this Window.
	 */
	GLXContext Window::getGLXContext() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return NULL;
		return glfwGetGLXContext(glfwWindow);
	}

	/**
	 *  @brief Get the GLX's window associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The GLX's window associated to this Window.
	 */
	GLXWindow Window::getGLXContext() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return None;
		return glfwGetGLXWindow(glfwWindow);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_EGL
	/**
	 *  @brief Get the EGL's context associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The EGL's context associated to this Window.
	 */
	EGLContext Window::getEGLContext() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return EGL_NO_CONTEXT;
		return glfwGetEGLContext(glfwWindow);
	}

	/**
	 *  @brief Get the EGL's surface associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The EGL's surface associated to this Window.
	 */
	EGLSurface Window::getEGLSurface() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return EGL_NO_SURFACE;
		return glfwGetEGLSurface(glfwWindow);
	}
#endif

#ifdef GLFW_EXPOSE_NATIVE_OSMESA
	/**
	 *  @brief Get the Mesa's color buffer associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return GLFW_TRUE if successful, GLFW_FALSE otherwise.
	 */
	int Window::getOSMesaColorBuffer(int* width, int* height, int* format, void** buffer) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return GLFW_FALSE;
		return glfwGetOSMesaColorBuffer(glfwWindow, width, height, format, buffer);
	}

	/**
	 *  @brief Get the Mesa's depth buffer associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return GLFW_TRUE if successful, GLFW_FALSE otherwise.
	 */
	int Window::getOSMesaDepthBuffer(int* width, int* height, int* bytesPerValue, void** buffer) const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return GLFW_FALSE;
		return glfwGetOSMesaDepthBuffer(glfwWindow, width, height, bytesPerValue, buffer);
	}

	/**
	 *  @brief Get the Mesa's OSMesaContext associated to this Window.
	 *  @note Be sure not to access such object after destroying this Window.
	 *  @return The Mesa's OSMesaContext associated to this Window.
	 */
	OSMesaContext Window::getOSMesaContext() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(mutexes[sharedMutexID].mutex);
#endif
		if (!glfwWindow)
			return NULL;
		return glfwGetOSMesaContext(glfwWindow);
	}
#endif

#ifndef NO_MULTITHREADING
	/**
	 *  @brief  Mutex used to guarantee correct concurrent management of static activities.
	 */
	std::recursive_mutex Window::globalMutex;

	/**
	 *  @brief  The container of mutexes.
	 */
	std::deque<Window::MutexData> Window::mutexes;

	/**
	 *  @brief  The container for reusing freed MutexIDs. It is kept sorted.
	 */
	std::deque<Window::MutexID> Window::freedMutexes;

	/**
	 *  @brief  The newMutexID static method books a new or an old & freed ID for mutexes.
	 *  @return The booked MutexID.
	 */
	Window::MutexID Window::newMutexID() {
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
	void Window::decreaseMutexCount(const MutexID id) {
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
	Window::WindowMap Window::windowsMap;

	/**
	 *  @brief  The container for collecting Windows.
	 */
	std::deque<WindowPointer> Window::windows;

	/**
	 *  @brief  The container for reusing freed WindowIDs.
	 */
	std::deque<WindowID> Window::freedWindowIDs;

	/**
	 *  @brief  The newWindowID static method books a new or an old & freed ID for windows.
	 *  @return The booked WindowID.
	 */
	WindowID Window::newWindowID() {
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
	 *  @brief  The newWindow static method allocates and creates a new Window, storing also internal pointers and
	 * information in order to easily retrieve it later.
	 *  @param width   The width of the new window, in screen space.
	 *  @param height  The height of the new window, in screen space.
	 *  @param title   The title of the new window.
	 *  @param monitor The monitor for a fullscreen window, or a null pointer for a non-fullscreen window.
	 *  @param share   The Window to share its context with, or a null pointer otherwise.
	 *  @return A WindowPointer to the new window.
	 */
	WindowPointer Window::newWindow(const int width,
	                                const int height,
	                                const std::string& title,
	                                GLFWmonitor* monitor,
	                                const WindowPointer& share) {
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
	WindowPointer Window::getWindow(const WindowID id) {
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
	WindowID Window::getWindowID(GLFWwindow* w) {
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
	void Window::getAllWindowIDs(std::unordered_set<WindowID>& wIDs) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		wIDs.clear();
		for (auto& w : windows)
			if (w)
				wIDs.insert(w->windowID);
	}

	/**
	 *  @brief  The isAnyWindowOpen static method says if there is any Window still open.
	 *  @return true if is there any Window still open, false otherwise.
	 */
	bool Window::isAnyWindowOpen() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		for (auto& w : windows)
			if (w && !w->shouldClose())
				return true;
		return false;
	}

	/**
	 *  @brief  The windowsToClose static method returns a set of Windows that have the close flag on.
	 *  @param wtc The set of WindowIDs of the Windows to close.
	 */
	void Window::windowsToClose(std::unordered_set<WindowID>& wtc) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		wtc.clear();
		for (auto& w : windows)
			if (w && w->shouldClose())
				wtc.insert(w->windowID);
	}

	/**
	 *  @brief  The deleteWindow static method destroys and removes the Window at id.
	 *  @param id The ID of the Window to remove.
	 */
	void Window::deleteWindow(const WindowID id) {
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
	void Window::deleteAllWindows() {
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
	void Window::unmapWindow(GLFWwindow* w) {
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
	void Window::freeWindowID(const WindowID id) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		std::deque<WindowID>::iterator pos = std::lower_bound(freedWindowIDs.begin(), freedWindowIDs.end(), id);
		freedWindowIDs.insert(pos, id);
	}

}
