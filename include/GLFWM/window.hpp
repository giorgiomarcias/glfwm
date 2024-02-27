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

#ifndef GLFWM_WINDOW_HPP
#define GLFWM_WINDOW_HPP

#include <GLFWM/drawable.hpp>
#include <GLFWM/event_handler.hpp>

namespace glfwm {

	class Window;

	/**
	 *  @brief  The WindowPointer is a smart pointer for a Window.
	 */
	using WindowPointer = std::shared_ptr<Window>;

	/**
	 *  @brief  The Window class represents a GLFWWindow with a set of threading facilities and some handlers for
	 * interactive objects, like event handlers or drawables.
	 */
	class Window {
	  public:
		/**
		 *  @brief  The type used for the rank determining the position in lists.
		 */
		using RankType = int;

		/**
		 *  @brief  Contructor. See newWindow for a reliable construction.
		 *  @param  id The ID of this new window. See newWindowID.
		 *  @param  width The width of the window area.
		 *  @param  height The height of the window area.
		 *  @param  title The title of the window.
		 *  @param  monitor The GLFWMonitor to associate this window to in case of fullscreen. Use nullptr otherwise.
		 *  @param  share Another Window to share the context with. Use nullptr for a non-shared context.
		 *  @note   The creation of a GLFW window may occur only in the main thread. Do not instantiate a Window in
		 * secondary threads.
		 */
		Window(const WindowID id,
		       const int width,
		       const int height,
		       const std::string& title,
		       GLFWmonitor* monitor = nullptr,
		       const WindowPointer& share = WindowPointer(nullptr));

		/**
		 *  @brief  The copy constructor is deleted, i.e. a Window can not be copied.
		 *  @param  The Window to copy.
		 */
		Window(const Window&) = delete;

		/**
		 *  @brief  The move constructor is deleted, i.e. a Window can not be moved.
		 *  @param  The Window to move.
		 */
		Window(Window&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. a Window can not be copied.
		 *  @param  The Window to copy.
		 *  @return A reference to this Window.
		 */
		Window& operator=(const Window&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. a Window can not be moved.
		 *  @param  The Window to move.
		 *  @return A reference to this Window.
		 */
		Window& operator=(Window&&) = delete;

		/**
		 *  @brief  Destructor.
		 */
		~Window();

		/**
		 *  @brief  The destroy method releases all GLFW data owned by this Window. This may only be called from the
		 * main thread.
		 */
		void destroy();

		/**
		 *  @brief  returns this Window's assigned ID.
		 *  @return this Window's assigned ID.
		 */
		WindowID getID() const;

		/**
		 *  @brief  The bindEventHandler method binds an EventHandler by adding it to the list of handlers in a position
		 * determined by the rank r.
		 *  @param eh The EventHandler to bind.
		 *  @param r  The rank associated to the EventHandler to bind. This determines its position in the list of
		 * handlers.
		 *  @note   If the handler has been already bound, it is rebound with the eventually new rank. Moreover, an
		 * handler may be bound to several windows.
		 */
		void bindEventHandler(const EventHandlerPointer& eh, const RankType r);

		/**
		 *  @brief  The unbindEventHandler methods removes the EventHandler eh from the list of handlers, if present.
		 *  @param eh The EventHandler to unbind.
		 */
		void unbindEventHandler(const EventHandlerPointer& eh);

		/**
		 *  @brief  The handleEvent method is called when an event occurs and it just calls the bound handlers in the
		 * order given by their rank until one of them positively handles it.
		 *  @param e The event to handle.
		 */
		void handleEvent(const EventPointer& e);

		/**
		 *  @brief  The bindDrawable method binds a Drawable by adding it to the list of drawable objects in a position
		 * determined by the rank r.
		 *  @param d The Drawable to bind.
		 *  @param r The rank associated to the Drawable to bind. This determines its position in the list of drawables.
		 *  @note   If the drawable has been already bound, it is rebound with the eventually new rank. Moreover, a
		 * drawable may be bound to several windows.
		 */
		void bindDrawable(const DrawablePointer& d, const RankType r);

		/**
		 *  @brief  The unbindDrawable methods removes the Drawable d from the list of drawable objects, if present.
		 *  @param d The Drawable to unbind.
		 */
		void unbindDrawable(const DrawablePointer& d);

		/**
		 *  @brief  The draw method is called when this window is rendered and it just calls the bound drawables in the
		 * order given by their rank.
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
		 *  @brief  The getTitle method returns the current title.
		 *  @note   This may only be called from the main thread.
		 *  @return This window's current title.
		 */
		std::string getTitle() const;

		/**
		 *  @brief  The setTitle method changes the current title.
		 *  @param title The new window title.
		 *  @note   This may only be called from the main thread.
		 */
		void setTitle(const std::string& title);

		/**
		 *  @brief  The getPosition method returns the x and y screen coordinate position.
		 *  @param x The x coordinate.
		 *  @param y The y coordintate.
		 *  @note   This may only be called from the main thread.
		 */
		void getPosition(int& x, int& y) const;

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
		void getSize(int& width, int& height) const;

		/**
		 *  @brief  The setSize method sets the width and height of this window area.
		 *  @param width The width of this window area.
		 *  @param height The height of this window area.
		 *  @note   This may only be called from the main thread.
		 */
		void setSize(const int width, const int height);

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
		void setSizeLimits(const int minWidth, const int minHeight, const int maxWidth, const int maxHeight);

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
		void setAspectRatio(const int numerator, const int denominator);

		/**
		 *  @brief  The getFramebufferSize method returns the width and height of this window framebuffer.
		 *  @param width The width of this window framebuffer.
		 *  @param height The height of this window framebuffer.
		 *  @note   This may only be called from the main thread.
		 */
		void getFramebufferSize(int& width, int& height) const;

		/**
		 *  @brief  The getFrameSize method returns size of each of the frame edges of this window.
		 *  @param left The left edge size.
		 *  @param top The top edge size.
		 *  @param right The right edge size.
		 *  @param bottom The bottom edge size.
		 *  @note   This may only be called from the main thread.
		 */
		void getFrameSize(int& left, int& top, int& right, int& bottom) const;

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
		void getContentScale(float& xScale, float& yScale) const;

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
		float getOpacity() const;

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
		void setOpacity(float opacity);

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
		void requestAttention();

		/**
		 *  @brief  The getInputMode method returns the current value of an input mode for this Window.
		 *  @param inputMode The input mode to query for its value.
		 *  @return The value of the specified input mode.
		 *  @note   This may only be called from the main thread.
		 */
		InputModeValueType getInputMode(const InputModeType inputMode) const;

		/**
		 *  @brief  The setInputMode method sets a new value for the specified input mode.
		 *  @param inputMode      The input mode whose value must be changed.
		 *  @param inputModeValue The value for the specified input mode.
		 *  @note   This may only be called from the main thread.
		 */
		void setInputMode(const InputModeType inputMode, const InputModeValueType inputModeValue);

		/**
		 *  @brief  The getKey method returns the last state reported for the specified key.
		 *  @param key  The key to query for its last state.
		 *  @return The last state of the specified key.
		 *  @note   This may only be called from the main thread.
		 */
		ActionType getKey(const KeyType key) const;

		/**
		 *  @brief  The getMouseButton method returns the last state reported for the specified mouse button.
		 *  @param mouseButton  The mouse button to query for its last state.
		 *  @return The last state of the specified mouse button.
		 *  @note   This may only be called from the main thread.
		 */
		ActionType getMouseButton(const MouseButtonType mouseButton) const;

		/**
		 *  @brief  The setCursor method sets the cursor shape to be used when the cursor is over this Window area.
		 *  @param cursor The cursor to set. If a null pointer is passed, the default arrow cursor is set.
		 *  @note   This may only be called from the main thread.
		 */
		void setCursor(GLFWcursor* cursor);

		/**
		 *  @brief  The getCursorPosition method returns the cursor position, in screen coordinates, relative to
		 *          the upper left corner of this window.
		 *  @param x The x-coordinate of the cursor.
		 *  @param y The y-coordinate of the cursor.
		 */
		void getCursorPosition(double& x, double& y) const;

		/**
		 *  @brief  The setCursorPosition method changes the cursor position, in screen coordinates, relative to
		 *          the upper left corner of this window.
		 *  @param x The new x-coordinate of the cursor.
		 *  @param y The new y-coordinate of the cursor.
		 */
		void setCursorPosition(double x, double y);

		/**
		 *  @brief  The setIcon method sets the icon of this window.
		 *
		 *  If passed an array of candidate images, those of or closest to the sizes desired by the system are selected.
		 *  If no images are specified, the window reverts to its default icon.
		 *  The desired image sizes varies depending on platform and system settings. The selected images will be
		 * rescaled as needed. Good sizes include 16x16, 32x32 and 48x48.
		 *
		 *  @param count  The number of images in the specified array, or zero to revert to the default window icon.
		 *  @param images The images to create the icon from. This is ignored if count is zero.
		 *  @note   On Mac OS X, the window has no icon, as it is not a document window, so this function does nothing.
		 *          The dock icon will be the same as the application bundle's icon.
		 *          This may only be called from the main thread.
		 */
		void setIcon(const int count, const GLFWimage* images);

		/**
		 *  @brief  The maximize method maximizes the specified window if it was previously not maximized.
		 *          If the window is already maximized, this function does nothing.
		 *  @note   This may only be called from the main thread.
		 */
		void maximize();

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
		 *  @brief  The focus method brings the specified window to front and sets input focus. By default,
		 *          both windowed and full screen mode windows are focused when initially created. Set the GLFW_FOCUSED
		 * to disable this behavior.
		 *  @note   The window should already be visible and not iconified. Do not use this function to steal focus
		 *          from other applications unless you are certain that is what the user wants.
		 *          Focus stealing can be extremely disruptive.
		 *          This may only be called from the main thread.
		 */
		void focus();

		/**
		 *  @brief  The getMonitor method returns the GLFWMonitor this window uses in fullscreen.
		 *  @return This window monitor.
		 *  @note   This may only be called from the main thread.
		 */
		GLFWmonitor* getMonitor() const;

		/**
		 *  @brief  The setMonitor method sets this window at full-screen (if monitor != nullptr) or windowed
		 * (otherwise).
		 *
		 *  When setting a monitor, this function updates the width, height and refresh rate of the desired video mode
		 *  and switches to the video mode closest to it.
		 *  The window position is ignored when setting a monitor.
		 *  When the monitor is nullptr, the position, width and height are used to place the window client area.
		 *  The refresh rate is ignored when no monitor is specified.
		 *  If you only wish to update the resolution of a full screen window or the size of a windowed mode window, see
		 * setSize. When a window transitions from full screen to windowed mode, this function restores any previous
		 * window settings such as whether it is decorated, floating, resizable, has size or aspect ratio limits, etc..
		 *
		 *  @param monitor      The desired monitor, or nullptr to set windowed mode.
		 *  @param xpos         The desired x-coordinate of the upper-left corner of the client area.
		 *  @param ypos         The desired y-coordinate of the upper-left corner of the client area.
		 *  @param width        The desired with, in screen coordinates, of the client area or video mode.
		 *  @param height       The desired height, in screen coordinates, of the client area or video mode.
		 *  @param refreshRate  The desired refresh rate, in Hz, of the video mode, or GLFW_DONT_CARE.
		 *  @note   This may only be called from the main thread.
		 */
		void setMonitor(GLFWmonitor* monitor,
		                const int xpos,
		                const int ypos,
		                const int width,
		                const int height,
		                const int refreshRate);

		/**
		 *  @brief  The getAttribute method returns this window attributes. See GLFW.
		 *  @param  attribute The attribute to read.
		 *  @return The value of attribute for this window.
		 *  @note   This may only be called from the main thread.
		 */
		int getAttribute(const int attribute) const;

		/**
		 *  @brief  The setAttribute method sets this window `attribute` to the specified `value`. See GLFW.
		 *  @param  attribute The attribute to write.
		 *  @param  The value of attribute to set for this window.
		 *  @note   This may only be called from the main thread.
		 */
		void setAttribute(const int attribute, const int value);

		/**
		 *  @brief  The getUserPointer method returns the user pointer for this window. See GLFW.
		 *  @return The user pointer.
		 */
		void* getUserPointer() const;

		/**
		 *  @brief  The setUserPointer method sets the user pointer for this window. See GLFW.
		 *  @param pointer The user pointer to set.
		 */
		void setUserPointer(void* pointer);

		/**
		 *  @brief  The getClipboardString method returns the system clipboard text, if any.
		 *  @param text The output system clipboard text.
		 *  @note   This may only be called from the main thread.
		 */
		void getClipboardString(std::string& text) const;

		/**
		 *  @brief  The setClipboardString method sets the system clipboard text.
		 *  @param text The system clipboard text to set.
		 *  @note   This may only be called from the main thread.
		 */
		void setClipboardString(const std::string& text);

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
		 *  @note   A call to this may only follow a call to makeContextCurrent, without nesting, otherwise the
		 * behaviour is undefined. This method does nothing if NO_MULTITHREADING is defined.
		 */
		void doneCurrentContext();

#ifdef VK_VERSION_1_0
		/**
		 *  @brief The createVulkanWindowSurface method creates a Vulkan surface for this window.
		 *
		 *  If the Vulkan loader was not found at initialization, this function returns VK_ERROR_INITIALIZATION_FAILED
		 *  and generates a GLFW_API_UNAVAILABLE error. Call glfwVulkanSupported to check whether the Vulkan loader was
		 * found. If the required window surface creation instance extensions are not available or if the specified
		 * instance was not created with these extensions enabled, this function returns VK_ERROR_EXTENSION_NOT_PRESENT
		 * and generates a GLFW_API_UNAVAILABLE error. Call glfwGetRequiredInstanceExtensions to check what instance
		 * extensions are required. The window surface must be destroyed before the specified Vulkan instance. It is the
		 * responsibility of the caller to destroy the window surface. GLFW(M) does not destroy it for you. Call
		 * vkDestroySurfaceKHR to destroy the surface.
		 *
		 *    @param instance  The Vulkan instance to create the surface in.
		 *    @param allocator The allocator to use, or null_ptr to use the default allocator.
		 *    @param surface   Where to store the handle of the surface. This is set to VK_NULL_HANDLE if an error
		 * occurred.
		 *    @return VK_SUCCESS if successful, or a Vulkan error code if an error occurred.
		 */
		VkResult createVulkanWindowSurface(VkInstance instance,
		                                   const VkAllocationCallbacks* allocator,
		                                   VkSurfaceKHR* surface);
#endif

#ifdef GLFW_EXPOSE_NATIVE_WIN32
		/**
		 *  @brief Get the Win32 handle associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The Win32 handle associated to this Window.
		 */
		HWND getWin32Window() const;
#endif

#ifdef GLFW_EXPOSE_NATIVE_COCOA
		/**
		 *  @brief Get the NSWindow associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The NSWindow associated to this Window.
		 */
		id getCocoaWindow() const;

		/**
		 *  @brief Get the NSView id associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The NSView id associated to this Window.
		 */
		id getCocoaView() const;
#endif

#ifdef GLFW_EXPOSE_NATIVE_X11
		/**
		 *  @brief Get the X11's Window associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The X11's Window associated to this Window.
		 */
		::Window getX11Widow() const;
#endif

#ifdef GLFW_EXPOSE_NATIVE_WAYLAND
		/**
		 *  @brief Get the Wayland's wl_surface associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The Wayland's wl_surface associated to this Window.
		 */
		struct wl_surface* getWaylandWidow() const;
#endif

#ifdef GLFW_EXPOSE_NATIVE_WGL
		/**
		 *  @brief Get the WGL's context associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The WGL's context associated to this Window.
		 */
		HGLRC getWGLContext() const;
#endif

#ifdef GLFW_EXPOSE_NATIVE_NSGL
		/**
		 *  @brief Get the NSOpenGLContext associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The NSOpenGLContext associated to this Window.
		 */
		id getNSGLContext() const;
#endif

#ifdef GLFW_EXPOSE_NATIVE_GLX
		/**
		 *  @brief Get the GLX's context associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The GLX's context associated to this Window.
		 */
		GLXContext getGLXContext() const;

		/**
		 *  @brief Get the GLX's window associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The GLX's window associated to this Window.
		 */
		GLXWindow getGLXWindow() const;
#endif

#ifdef GLFW_EXPOSE_NATIVE_EGL
		/**
		 *  @brief Get the EGL's context associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The EGL's context associated to this Window.
		 */
		EGLContext getEGLContext() const;

		/**
		 *  @brief Get the EGL's surface associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The EGL's surface associated to this Window.
		 */
		EGLSurface getEGLSurface() const;
#endif

#ifdef GLFW_EXPOSE_NATIVE_OSMESA
		/**
		 *  @brief Get the Mesa's color buffer associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return GLFW_TRUE if successful, GLFW_FALSE otherwise.
		 */
		int getOSMesaColorBuffer(int* width, int* height, int* format, void** buffer) const;

		/**
		 *  @brief Get the Mesa's depth buffer associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return GLFW_TRUE if successful, GLFW_FALSE otherwise.
		 */
		int getOSMesaDepthBuffer(int* width, int* height, int* bytesPerValue, void** buffer) const;

		/**
		 *  @brief Get the Mesa's OSMesaContext associated to this Window.
		 *  @note Be sure not to access such object after destroying this Window.
		 *  @return The Mesa's OSMesaContext associated to this Window.
		 */
		OSMesaContext getOSMesaContext() const;
#endif

	  private:
		// The WindowManager is friend for letting it access this Window's private data member glfwWindow.
		friend class WindowManager;

		/**
		 *  @brief  The GLFW window data structure for this Window and its context.
		 */
		GLFWwindow* glfwWindow;

		/**
		 *  @brief  The WindowID is the ID of this window, which is assigned at construction time
		 *          and remains constant until destruction.
		 */
		const WindowID windowID;

		/**
		 *  @brief  The EventHandlerRank struct stores a pointer to an EventHandler and its rank which determines its
		 * position in the list.
		 */
		using EventHandlerRank = ObjectRank<EventHandlerPointer, RankType>;

		/**
		 *  @brief  The list of EventHandler pointers sorted by their respective rank, used to determine which one will
		 * handle an event first.
		 */
		std::deque<EventHandlerRank> eventHandlers;

		/**
		 *  @brief  The EventHandlersIterator is an iterator to an element of eventHandlers.
		 */
		using EventHandlersIterator = std::deque<EventHandlerRank>::iterator;

		/**
		 *  @brief  The type of the container used to store and fast retrieve the handler position in the list.
		 */
		using EventHandlerMap = std::unordered_map<EventHandlerPointer, EventHandlersIterator>;

		/**
		 *  @brief  The iterator used to search for elements in the EventHandlerMap.
		 */
		using EventHandlerMapIterator = EventHandlerMap::iterator;

		/**
		 *  @brief  The container for storing EventHandler objects currently bound and associated position in list.
		 */
		EventHandlerMap eventHandlerMap;

		/**
		 *  @brief  The DrawableRank struct stores a pointer to an Drawable and its rank which determines its position
		 * in the list.
		 */
		using DrawableRank = ObjectRank<DrawablePointer, RankType>;

		/**
		 *  @brief  The list of Drawables pointers sorted by their respective rank, used to determine which one will
		 * draw first.
		 */
		std::deque<DrawableRank> drawables;

		/**
		 *  @brief  The DrawablesIterator is an iterator to an element of drawables.
		 */
		using DrawablesIterator = std::deque<DrawableRank>::iterator;

		/**
		 *  @brief  The type of the container used to store and fast retrieve the drawable position in the list.
		 */
		using DrawableMap = std::unordered_map<DrawablePointer, DrawablesIterator>;

		/**
		 *  @brief  The iterator used to search for elements in the DrawableMap.
		 */
		using DrawableMapIterator = DrawableMap::iterator;

		/**
		 *  @brief  The container for storing Drawable objects currently bound and associated position in list.
		 */
		DrawableMap drawableMap;

#ifndef NO_MULTITHREADING
		/**
		 *  @brief  Mutex used to guarantee correct concurrent management of static activities.
		 */
		static std::recursive_mutex globalMutex;

		/**
		 *  @brief  The type of the ID used to identify a mutex.
		 */
		using MutexID = size_t;

		/**
		 *  @brief  The ID to identify a mutex. It is actually an index of the mutexes container.
		 */
		MutexID sharedMutexID;

		/**
		 *  @brief  The MutexData struct is just a wrapper for a mutex together with its number of users.
		 */
		struct MutexData {
			std::recursive_mutex mutex;
			size_t count;
			MutexData() : count(0) {}
		};

		/**
		 *  @brief  The container of mutexes.
		 */
		static std::deque<MutexData> mutexes;

		/**
		 *  @brief  The container for reusing freed MutexIDs. It is kept sorted.
		 */
		static std::deque<MutexID> freedMutexes;

		/**
		 *  @brief  The newMutexID static method books a new or an old & freed ID for mutexes.
		 *  @return The booked MutexID.
		 */
		static MutexID newMutexID();

		/**
		 *  @brief  The decreaseMutexCount static method decreases the number of users of a mutex, eventually freeing
		 * it.
		 *  @param id The ID of the mutex.
		 */
		static void decreaseMutexCount(const MutexID id);
#endif

		/**
		 *  @brief  The WindowMap class is a hash table used to map GLFWWindow pointers to Window IDs.
		 */
		using WindowMap = std::unordered_map<GLFWwindow*, WindowID>;

		/**
		 *  @brief  The iterator for accessing the WindowMap.
		 */
		using WindowMapIterator = WindowMap::iterator;

		/**
		 *  @brief  The map between GLFWWindow pointers and WindowIDs.
		 */
		static WindowMap windowsMap;

		/**
		 *  @brief  The container for collecting Windows.
		 */
		static std::deque<WindowPointer> windows;

		/**
		 *  @brief  The container for reusing freed WindowIDs. It is kept sorted.
		 */
		static std::deque<WindowID> freedWindowIDs;

	  public:
		/**
		 *  @brief  The newWindowID static method books a new or an old & freed ID for windows.
		 *  @return The booked WindowID.
		 */
		static WindowID newWindowID();

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
		static WindowPointer newWindow(const int width,
		                               const int height,
		                               const std::string& title,
		                               GLFWmonitor* monitor = nullptr,
		                               const WindowPointer& share = WindowPointer(nullptr));

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
		static WindowID getWindowID(GLFWwindow* w);

		/**
		 *  @brief  The getALlWindowIDs static method returns the set of all the WindowID currently in use.
		 *  @param wIDs The set of WindowIDs.
		 */
		static void getAllWindowIDs(std::unordered_set<WindowID>& wIDs);

		/**
		 *  @brief  The isAnyWindowOpen static method says if there is any Window still open.
		 *  @return true if is there any Window still open, false otherwise.
		 */
		static bool isAnyWindowOpen();

		/**
		 *  @brief  The windowsToClose static method returns a set of Windows that have the close flag on.
		 *  @param wtc The set of WindowIDs of the Windows to close.
		 */
		static void windowsToClose(std::unordered_set<WindowID>& wtc);

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
		static void unmapWindow(GLFWwindow* w);

		/**
		 *  @brief  The freeWindowID frees an ID for a later reuse.
		 *  @param id The ID to free.
		 */
		static void freeWindowID(const WindowID id);
	};

}

#endif
