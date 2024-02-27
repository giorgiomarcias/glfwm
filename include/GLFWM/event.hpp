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

#ifndef GLFWM_EVENT_HPP
#define GLFWM_EVENT_HPP

#include <GLFWM/enums.hpp>

namespace glfwm {

	/**
	 *  @brief  The Event class is the base class representing an abstract event.
	 */
	class Event {
	  public:
		/**
		 *  @brief Default constructor for an EventType::EMPTY Event.
		 */
		Event();

		/**
		 *    @brief  Virtual destructor for making this class polymorphic
		 */
		virtual ~Event() {}

		/**
		 *  @brief  Initializer constructor.
		 *  @param  wID The ID of the window, or AllWindowIDs for every Window, or WholeGroupWindowIDs for all the
		 * Windows of the target group.
		 *  @param  t The EventType for this Event.
		 */
		Event(const WindowID wID, const EventType t);

		/**
		 *  @brief Copy constructor.
		 *  @param e The Event to copy.
		 */
		Event(const Event& e);

		/**
		 *  @brief The move constructor is deleted, i.e. an Event can not be moved.
		 *  @param The Event to move.
		 */
		Event(Event&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an Event can not be copied.
		 *  @param  The event to copy.
		 *  @return A reference to this Event.
		 */
		Event& operator=(const Event&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an Event can not be moved.
		 *  @param  The Event to move.
		 *  @return A reference to this Event.
		 */
		Event& operator=(Event&&) = delete;

		/**
		 *  @brief  Returns the ID of the window this event refers to, or the special AllWindowIDs, or
		 * WholeGroupWindowIDs.
		 *  @return The ID of the window.
		 */
		WindowID getWindowID() const;

		/**
		 *  @brief  The getEventType method returns this Event's EventType.
		 *  @return This Event's EventType.
		 */
		EventType getEventType() const;

	  private:
		/**
		 *  @brief  The ID of the window this event refers to, or the special values AllWindowIDs, WholeGroupWindowIDs.
		 */
		const WindowID windowID;

		/**
		 *  @brief  The EventType of this Event.
		 */
		const EventType eventType;
	};

	/**
	 *  @brief  The EventPointer is a smart pointer to an Event object (or a derived one).
	 */
	using EventPointer = std::shared_ptr<Event>;

	/**
	 *  @brief  The EventWindowsPosition class represents a windows position change event.
	 */
	class EventWindowPosition : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventWindowPosition();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  x The new x coordinate.
		 *  @param  y The new y coordinate.
		 */
		EventWindowPosition(const WindowID id, const int x, const int y);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ewp The EventWindowPosition to copy.
		 */
		EventWindowPosition(const EventWindowPosition& ewp);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventWindowPosition can not be moved.
		 *  @param  The EventWindowPosition to move.
		 */
		EventWindowPosition(EventWindowPosition&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventWindowPosition can not be copied.
		 *  @param  The EventWindowPosition to copy.
		 *  @return A reference to this EventWindowPosition.
		 */
		EventWindowPosition& operator=(const EventWindowPosition&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventWindowPosition can not be moved.
		 *  @param  The EventWindowPosition to move.
		 *  @return A reference to this EventWindowPosition.
		 */
		EventWindowPosition& operator=(EventWindowPosition&&) = delete;

		/**
		 *  @brief  Returns the new x coordinate for this window position change event.
		 *  @return The new x coordinate.
		 */
		int getX() const;

		/**
		 *  @brief  Returns the new y coordinate for this window position change event.
		 *  @return The new y coordinate.
		 */
		int getY() const;

	  private:
		/**
		 *  @brief  The x coordinate associated to this window position change event.
		 */
		const int xPos;

		/**
		 *  @brief  The y coordinate associated to this window position change event.
		 */
		const int yPos;
	};

	/**
	 *  @brief  The EventWindowSize class represents a window's size change event.
	 */
	class EventWindowSize : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventWindowSize();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  w The new width.
		 *  @param  h The new height.
		 */
		EventWindowSize(const WindowID id, const int w, const int h);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ews The EventWindowSize to copy.
		 */
		EventWindowSize(const EventWindowSize& ews);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventWindowSize can not be moved.
		 *  @param  The EventWindowSize to move.
		 */
		EventWindowSize(EventWindowSize&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventWindowSize can not be copied.
		 *  @param  The EventWindowSize to copy.
		 *  @return A reference to this EventWindowSize.
		 */
		EventWindowSize& operator=(const EventWindowSize&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventWindowSize can not be moved.
		 *  @param  The EventWindowSize to move.
		 *  @return A reference to this EventWindowSize.
		 */
		EventWindowSize& operator=(EventWindowSize&&) = delete;

		/**
		 *  @brief  Returns the new width of this window size change event.
		 *  @return The new width coordinate.
		 */
		int getWidth() const;

		/**
		 *  @brief  Returns the new height of this window size change event.
		 *  @return The new height coordinate.
		 */
		int getHeight() const;

	  private:
		/**
		 *  @brief  The width associated to this window size change event.
		 */
		const int width;

		/**
		 *  @brief  The height associated to this window size event.
		 */
		const int height;
	};

	/**
	 *  @brief  The EventWindowClose class represents a window's close request event.
	 */
	class EventWindowClose : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventWindowClose();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 */
		EventWindowClose(const WindowID id);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ewc The EventWindowClose to copy.
		 */
		EventWindowClose(const EventWindowClose& ewc);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventWindowClose can not be moved.
		 *  @param  The EventWindowClose to move.
		 */
		EventWindowClose(EventWindowClose&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventWindowClose can not be copied.
		 *  @param  The EventWindowClose to copy.
		 *  @return A reference to this EventWindowClose.
		 */
		EventWindowClose& operator=(const EventWindowClose&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventWindowClose can not be moved.
		 *  @param  The EventWindowClose to move.
		 *  @return A reference to this EventWindowClose.
		 */
		EventWindowClose& operator=(EventWindowClose&&) = delete;
	};

	/**
	 *  @brief  The EventWindowRefresh class represents a window's refresh request event.
	 */
	class EventWindowRefresh : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventWindowRefresh();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 */
		EventWindowRefresh(const WindowID id);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ewr The EventWindowRefresh to copy.
		 */
		EventWindowRefresh(const EventWindowRefresh& ewr);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventWindowRefresh can not be moved.
		 *  @param  ewp The EventWindowRefresh to move.
		 */
		EventWindowRefresh(EventWindowRefresh&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventWindowRefresh can not be copied.
		 *  @param  The EventWindowRefresh to copy.
		 *  @return A reference to this EventWindowRefresh.
		 */
		EventWindowRefresh& operator=(const EventWindowRefresh&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventWindowRefresh can not be moved.
		 *  @param  The EventWindowRefresh to move.
		 *  @return A reference to this EventWindowRefresh.
		 */
		EventWindowRefresh& operator=(EventWindowRefresh&&) = delete;
	};

	/**
	 *  @brief  The EventWindowFocus class represents a window's got or lost focus event.
	 */
	class EventWindowFocus : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventWindowFocus();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  f true for got focus, false for lost focus.
		 */
		EventWindowFocus(const WindowID id, const bool f);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ewf The EventWindowFocus to copy.
		 */
		EventWindowFocus(const EventWindowFocus& ewf);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventWindowFocus can not be moved.
		 *  @param  The EventWindowFocus to move.
		 */
		EventWindowFocus(EventWindowFocus&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventWindowFocus can not be copied.
		 *  @param  The EventWindowFocus to copy.
		 *  @return A reference to this EventWindowFocus.
		 */
		EventWindowFocus& operator=(const EventWindowFocus&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventWindowFocus can not be moved.
		 *  @param  The EventWindowFocus to move.
		 *  @return A reference to this EventWindowFocus.
		 */
		EventWindowFocus& operator=(EventWindowFocus&&) = delete;

		/**
		 *  @brief  The hasFocus method says whether this event corresponds to a got or lost focus.
		 *  @return true if the window has got focus, false if it has lost focus.
		 */
		bool hasFocus() const;

	  private:
		/**
		 *  @brief  The kind of focus activity associated to this event: got or lost focus.
		 */
		const bool focus;
	};

	/**
	 *  @brief  The EventWindowMaximize class represents a window's maximize or restore request event.
	 */
	class EventWindowMaximize : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventWindowMaximize();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  m true for maximizing, false for restoring.
		 */
		EventWindowMaximize(const WindowID id, const bool m);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ewm The EventWindowMaximize to copy.
		 */
		EventWindowMaximize(const EventWindowMaximize& ewm);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventWindowMaximize can not be moved.
		 *  @param  The EventWindowMaximize to move.
		 */
		EventWindowMaximize(EventWindowMaximize&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventWindowMaximize can not be copied.
		 *  @param  The EventWindowMaximize to copy.
		 *  @return A reference to this EventWindowMaximize.
		 */
		EventWindowMaximize& operator=(const EventWindowMaximize&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventWindowMaximize can not be moved.
		 *  @param  The EventWindowMaximize to move.
		 *  @return A reference to this EventWindowMaximize.
		 */
		EventWindowMaximize& operator=(EventWindowMaximize&&) = delete;

		/**
		 *  @brief  The toMaximize method says whether this event corresponds to a maximize or restore request.
		 *  @return true if the window should be maximizede, false if it should be restored.
		 */
		bool toMaximize() const;

	  private:
		/**
		 *  @brief  The kind of activity associated to this event: maximize or restore.
		 */
		const bool maximize;
	};

	/**
	 *  @brief  The EventWindowIconify class represents a window's iconify or restore request event.
	 */
	class EventWindowIconify : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventWindowIconify();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  i true for iconifying, false for restoring.
		 */
		EventWindowIconify(const WindowID id, const bool i);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ewi The EventWindowIconify to copy.
		 */
		EventWindowIconify(const EventWindowIconify& ewi);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventWindowIconify can not be moved.
		 *  @param  The EventWindowIconify to move.
		 */
		EventWindowIconify(EventWindowIconify&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventWindowIconify can not be copied.
		 *  @param  The EventWindowIconify to copy.
		 *  @return A reference to this EventWindowIconify.
		 */
		EventWindowIconify& operator=(const EventWindowIconify&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventWindowIconify can not be moved.
		 *  @param  The EventWindowIconify to move.
		 *  @return A reference to this EventWindowIconify.
		 */
		EventWindowIconify& operator=(EventWindowIconify&&) = delete;

		/**
		 *  @brief  The toIconify method says whether this event corresponds to a iconify or restore request.
		 *  @return true if the window should be iconified, false if it should be restored.
		 */
		bool toIconify() const;

	  private:
		/**
		 *  @brief  The kind of activity associated to this event: iconify or restore.
		 */
		const bool iconify;
	};

	/**
	 *  @brief  The EventFrameBufferSize class represents a window's framebuffer size change event.
	 */
	class EventFrameBufferSize : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventFrameBufferSize();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  w The new width.
		 *  @param  h The new height.
		 */
		EventFrameBufferSize(const WindowID id, const int w, const int h);

		/**
		 *  @brief  Copy constructor.
		 *  @param  efs The EventFrameBufferSize to copy.
		 */
		EventFrameBufferSize(const EventFrameBufferSize& efs);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventFrameBufferSize can not be moved.
		 *  @param  The EventFrameBufferSize to move.
		 */
		EventFrameBufferSize(EventFrameBufferSize&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventFrameBufferSize can not be copied.
		 *  @param  The EventFrameBufferSize to copy.
		 *  @return A reference to this EventFrameBufferSize.
		 */
		EventFrameBufferSize& operator=(const EventFrameBufferSize&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventFrameBufferSize can not be moved.
		 *  @param  The EventFrameBufferSize to move.
		 *  @return A reference to this EventFrameBufferSize.
		 */
		EventFrameBufferSize& operator=(EventFrameBufferSize&&) = delete;

		/**
		 *  @brief  Returns the new width of this window framebuffer size change event.
		 *  @return The new width coordinate.
		 */
		int getWidth() const;

		/**
		 *  @brief  Returns the new height of this window framebuffer size change event.
		 *  @return The new height coordinate.
		 */
		int getHeight() const;

	  private:
		/**
		 *  @brief  The width associated to this window framebuffer size change event.
		 */
		const int width;

		/**
		 *  @brief  The height associated to this window framebuffer size event.
		 */
		const int height;
	};

	/**
	 *  @brief  The EventContentScale class represents a window's content scale change event.
	 */
	class EventContentScale : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventContentScale();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  x The new scale in x.
		 *  @param  y The new scale in y.
		 */
		EventContentScale(const WindowID id, const float x, const float y);

		/**
		 *  @brief  Copy constructor.
		 *  @param  efs The EventContentScale to copy.
		 */
		EventContentScale(const EventContentScale& efs);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventContentScale can not be moved.
		 *  @param  The EventContentScale to move.
		 */
		EventContentScale(EventContentScale&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventContentScale can not be copied.
		 *  @param  The EventContentScale to copy.
		 *  @return A reference to this EventContentScale.
		 */
		EventContentScale& operator=(const EventContentScale&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventContentScale can not be moved.
		 *  @param  The EventContentScale to move.
		 *  @return A reference to this EventContentScale.
		 */
		EventContentScale& operator=(EventContentScale&&) = delete;

		/**
		 *  @brief  Returns the new scale in x of this window content scale change event.
		 *  @return The new scale in x.
		 */
		float getXScale() const;

		/**
		 *  @brief  Returns the new scale in y of this window content scale change event.
		 *  @return The new scale in y.
		 */
		float getYScale() const;

	  private:
		/**
		 *  @brief  The scale in x associated to this window content scale change event.
		 */
		const float xScale;

		/**
		 *  @brief  The scale in y associated to this window content scale event.
		 */
		const float yScale;
	};

	/**
	 *  @brief  The EventChar class represents a Unicode character input event.
	 */
	class EventChar : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventChar();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  c The new Unicode character.
		 */
		EventChar(const WindowID id, const char32_t c);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ec The EventChar to copy.
		 */
		EventChar(const EventChar& ec);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventChar can not be moved.
		 *  @param  The EventChar to move.
		 */
		EventChar(EventChar&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventChar can not be copied.
		 *  @param  The EventChar to copy.
		 *  @return A reference to this EventChar.
		 */
		EventChar& operator=(const EventChar&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventChar can not be moved.
		 *  @param  The EventChar to move.
		 *  @return A reference to this EventChar.
		 */
		EventChar& operator=(EventChar&&) = delete;

		/**
		 *  @brief  Returns the new Unicode codepoint of this input character event.
		 *  @return The new input character.
		 */
		char32_t getCharacter() const;

	  private:
		/**
		 *  @brief  The Unicode codepoint associated to this input character event.
		 */
		const char32_t character;
	};

	/**
	 *  @brief  The EventCharMod class represents a Unicode character with a modifier key input event
	 */
	class EventCharMod : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventCharMod();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  c The new Unicode character.
		 *  @param  m The new modifier keys (as a bit set of ModifierKeyType values).
		 */
		EventCharMod(const WindowID id, const char32_t c, const ModifierKeyBaseType m);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ecm The EventChar to copy.
		 */
		EventCharMod(const EventCharMod& ecm);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventCharMod can not be moved.
		 *  @param  The EventCharMod to move.
		 */
		EventCharMod(EventCharMod&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventCharMod can not be copied.
		 *  @param  The EventCharMod to copy.
		 *  @return A reference to this EventCharMod.
		 */
		EventCharMod& operator=(const EventCharMod&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventCharMod can not be moved.
		 *  @param  The EventCharMod to move.
		 *  @return A reference to this EventCharMod.
		 */
		EventCharMod& operator=(EventCharMod&&) = delete;

		/**
		 *  @brief  Returns the new Unicode codepoint of this input event.
		 *  @return The new input character.
		 */
		char32_t getCharacter() const;

		/**
		 *  @brief  Returns the modifier keys of this input event.
		 *  @return The input modifier keys (as a bit set of ModifierKeyType values).
		 */
		ModifierKeyBaseType getModifierKeys() const;

	  private:
		/**
		 *  @brief  The Unicode codepoint associated to this input event.
		 */
		const char32_t character;

		/**
		 *  @brief  The modifier keys (as a bit set of ModifierKeyType values) associated to this input event.
		 */
		const ModifierKeyBaseType modifierKeys;
	};

	/**
	 *  @brief  The EventCursorEnter class represents a cursor entering or exiting the window boundaries event.
	 */
	class EventCursorEnter : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventCursorEnter();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  e true for entering, false for exiting.
		 */
		EventCursorEnter(const WindowID id, const bool e);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ece The EventCursorEnter to copy.
		 */
		EventCursorEnter(const EventCursorEnter& ece);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventCursorEnter can not be moved.
		 *  @param  The EventCursorEnter to move.
		 */
		EventCursorEnter(EventCursorEnter&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventCursorEnter can not be copied.
		 *  @param  The EventCursorEnter to copy.
		 *  @return A reference to this EventCursorEnter.
		 */
		EventCursorEnter& operator=(const EventCursorEnter&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventCursorEnter can not be moved.
		 *  @param  The EventCursorEnter to move.
		 *  @return A reference to this EventCursorEnter.
		 */
		EventCursorEnter& operator=(EventCursorEnter&&) = delete;

		/**
		 *  @brief  The isEntering method says whether this event corresponds to an cursor entering or exiting event.
		 *  @return true if the cursor is entering, false if it is exiting.
		 */
		bool isEntering() const;

	  private:
		/**
		 *  @brief  The kind of activity associated to this event: entering or exiting.
		 */
		const bool enter;
	};

	/**
	 *  @brief  The EventCursorPosition class represents a cursor movement event.
	 */
	class EventCursorPosition : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventCursorPosition();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  x The new x coordinate.
		 *  @param  y The new y coordinate.
		 */
		EventCursorPosition(const WindowID id, const double x, const double y);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ecp The EventCursorPosition to copy.
		 */
		EventCursorPosition(const EventCursorPosition& ecp);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventCursorPosition can not be moved.
		 *  @param  The EventCursorPosition to move.
		 */
		EventCursorPosition(EventCursorPosition&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventCursorPosition can not be copied.
		 *  @param  The EventCursorPosition to copy.
		 *  @return A reference to this EventCursorPosition.
		 */
		EventCursorPosition& operator=(const EventCursorPosition&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventCursorPosition can not be moved.
		 *  @param  The EventCursorPosition to move.
		 *  @return A reference to this EventCursorPosition.
		 */
		EventCursorPosition& operator=(EventCursorPosition&&) = delete;

		/**
		 *  @brief  Returns the new x coordinate for this cursor position change event.
		 *  @return The new x coordinate.
		 */
		double getX() const;

		/**
		 *  @brief  Returns the new y coordinate for this cursor position change event.
		 *  @return The new y coordinate.
		 */
		double getY() const;

	  private:
		/**
		 *  @brief  The x coordinate associated to this cursor position change event.
		 */
		const double xPos;

		/**
		 *  @brief  The y coordinate associated to this cursor position change event.
		 */
		const double yPos;
	};

	/**
	 *  @brief  The EventDrop class represents a drop event.
	 */
	class EventDrop : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventDrop();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  ps The new Unicode string paths.
		 */
		EventDrop(const WindowID id, const std::list<std::string>& ps);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ed The EventDrop to copy.
		 */
		EventDrop(const EventDrop& ed);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventDrop can not be moved.
		 *  @param  The EventDrop to move.
		 */
		EventDrop(EventDrop&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventDrop can not be copied.
		 *  @param  The EventDrop to copy.
		 *  @return A reference to this EventDrop.
		 */
		EventDrop& operator=(const EventDrop&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventDrop can not be moved.
		 *  @param  The EventDrop to move.
		 *  @return A reference to this EventDrop.
		 */
		EventDrop& operator=(EventDrop&&) = delete;

		/**
		 *  @brief  Returns the new Unicode strings of this input drop event.
		 *  @return The new string paths input drop.
		 */
		const std::list<std::string>& getPaths() const;

	  private:
		/**
		 *  @brief  The Unicode strings associated to this input drop event.
		 */
		const std::list<std::string> paths;
	};

	/**
	 *  @brief  The EventKey class represents a key press, release or repeat event.
	 */
	class EventKey : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventKey();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  k The key identifier.
		 *  @param  s The system-specific scancode of this key.
		 *  @param  a The action (press, release or repeat).
		 *  @param  m The new modifier keys (as a bit set of ModifierKeyType values).
		 */
		EventKey(const WindowID id, const KeyType k, const char32_t s, const ActionType a, const ModifierKeyBaseType m);

		/**
		 *  @brief  Copy constructor.
		 *  @param  ek The EventKey to copy.
		 */
		EventKey(const EventKey& ek);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventKey can not be moved.
		 *  @param  The EventKey to move.
		 */
		EventKey(EventKey&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventKey can not be copied.
		 *  @param  The EventKey to copy.
		 *  @return A reference to this EventKey.
		 */
		EventKey& operator=(const EventKey&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventKey can not be moved.
		 *  @param  The EventKey to move.
		 *  @return A reference to this EventKey.
		 */
		EventKey& operator=(EventKey&&) = delete;

		/**
		 *  @brief  Returns the key identifier for this key event.
		 *  @return The key identifier.
		 */
		KeyType getKey() const;

		/**
		 *  @brief  Returns the system-specific scancode for this key event.
		 *  @return The system-specific scancode of the key.
		 */
		char32_t getScancode() const;

		/**
		 *  @brief  Returns the action (press, release or repeat) for this key event.
		 *  @return The action of the key.
		 */
		ActionType getAction() const;

		/**
		 *  @brief  Returns the modifier keys of this key event.
		 *  @return The input modifier keys (as a bit set of ModifierKeyType values).
		 */
		ModifierKeyBaseType getModifierKeys() const;

	  private:
		/**
		 *  @brief  The key associated to this key event.
		 */
		const KeyType key;

		/**
		 *  @brief  The system-specific scancode associated to this key event.
		 */
		const char32_t scancode;

		/**
		 *  @brief  The type of action (press, release or repeat) associated to this key event.
		 */
		const ActionType action;

		/**
		 *  @brief  The modifier keys (as a bit set of ModifierKeyType values) associated to this key event.
		 */
		const ModifierKeyBaseType modifierKeys;
	};

	/**
	 *  @brief  The EventMouseButton class represents a mouse button press or release event.
	 */
	class EventMouseButton : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventMouseButton();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  mb The key identifier.
		 *  @param  a The action (press, release or repeat).
		 *  @param  m The new modifier keys (as a bit set of ModifierKeyType values).
		 */
		EventMouseButton(const WindowID id, const MouseButtonType mb, const ActionType a, const ModifierKeyBaseType m);

		/**
		 *  @brief  Copy constructor.
		 *  @param  emb The EventMouseButton to copy.
		 */
		EventMouseButton(const EventMouseButton& emb);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventMouseButton can not be moved.
		 *  @param  The EventMouseButton to move.
		 */
		EventMouseButton(EventMouseButton&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventMouseButton can not be copied.
		 *  @param  The EventMouseButton to copy.
		 *  @return A reference to this EventMouseButton.
		 */
		EventMouseButton& operator=(const EventMouseButton&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventMouseButton can not be moved.
		 *  @param  The EventMouseButton to move.
		 *  @return A reference to this EventMouseButton.
		 */
		EventMouseButton& operator=(EventMouseButton&&) = delete;

		/**
		 *  @brief  Returns the button identifier for this mouse button event.
		 *  @return The button identifier.
		 */
		MouseButtonType getMouseButton() const;

		/**
		 *  @brief  Returns the action (press, release or repeat) for this mouse button event.
		 *  @return The action of the mouse button.
		 */
		ActionType getAction() const;

		/**
		 *  @brief  Returns the modifier keys of this mouse button event.
		 *  @return The input modifier keys (as a bit set of ModifierKeyType values).
		 */
		ModifierKeyBaseType getModifierKeys() const;

	  private:
		/**
		 *  @brief  The key associated to this key event.
		 */
		const MouseButtonType mouseButton;

		/**
		 *  @brief  The type of action (press, release or repeat) associated to this mouse event.
		 */
		const ActionType action;

		/**
		 *  @brief  The modifier keys (as a bit set of ModifierKeyType values) associated to this mouse event.
		 */
		const ModifierKeyBaseType modifierKeys;
	};

	/**
	 *  @brief  The EventScroll class represents a scroll event.
	 */
	class EventScroll : public Event {
	  public:
		/**
		 *  @brief  Default constructor.
		 */
		EventScroll();

		/**
		 *  @brief  Initializer constructor.
		 *  @param  id The ID of the window.
		 *  @param  x The x offset.
		 *  @param  y The y offset.
		 */
		EventScroll(const WindowID id, const double x, const double y);

		/**
		 *  @brief  Copy constructor.
		 *  @param  es The EventScroll to copy.
		 */
		EventScroll(const EventScroll& es);

		/**
		 *  @brief  The move constructor is deleted, i.e. an EventScroll can not be moved.
		 *  @param  The EventScroll to move.
		 */
		EventScroll(EventScroll&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. an EventScroll can not be copied.
		 *  @param  The EventScroll to copy.
		 *  @return A reference to this EventScroll.
		 */
		EventScroll& operator=(const EventScroll&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. an EventScroll can not be moved.
		 *  @param  The EventScroll to move.
		 *  @return A reference to this EventScroll.
		 */
		EventScroll& operator=(EventScroll&&) = delete;

		/**
		 *  @brief  Returns the x offset for this scroll event.
		 *  @return The x offset.
		 */
		double getX() const;

		/**
		 *  @brief  Returns the y offset for this scroll event.
		 *  @return The y offset.
		 */
		double getY() const;

	  private:
		/** @brief  The x offset associated to this scroll event.
		 */
		const double xOffset;

		/**
		 *  @brief  The y offset associated to this scroll event.
		 */
		const double yOffset;
	};

}

#endif
