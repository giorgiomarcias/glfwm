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

#include <GLFWM/event.hpp>

namespace glfwm {

	/**
	 *  @brief Default constructor for an EventType::EMPTY Event.
	 */
	Event::Event() : windowID(AllWindowIDs), eventType(EventType::EMPTY) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  wID The ID of the window, or AllWindowIDs for every Window, or WholeGroupWindowIDs for all the Windows
	 * of the target group.
	 *  @param  t The EventType for this Event.
	 */
	Event::Event(const WindowID id, const EventType t) : windowID(id), eventType(t) {}

	/**
	 *  @brief Copy constructor.
	 *  @param e The Event to copy.
	 */
	Event::Event(const Event& e) : windowID(e.windowID), eventType(e.eventType) {}

	/**
	 *  @brief  Returns the ID of the window this event refers to, or the special AllWindowIDs.
	 *  @return The ID of the window.
	 */
	WindowID Event::getWindowID() const { return windowID; }

	/**
	 *  @brief  The getEventType method returns this Event's EventType.
	 *  @return This Event's EventType.
	 */
	EventType Event::getEventType() const { return eventType; }

	/**
	 *  @brief  Default constructor.
	 */
	EventWindowPosition::EventWindowPosition() : Event(AllWindowIDs, EventType::WINDOW_POSITION), xPos(0), yPos(0) {}

	/**
	 *  @brief  Initializer constructor.

	 */
	EventWindowPosition::EventWindowPosition(const WindowID id, const int x, const int y)
	    : Event(id, EventType::WINDOW_POSITION), xPos(x), yPos(y) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ewp The EventWindowPosition to copy.
	 */
	EventWindowPosition::EventWindowPosition(const EventWindowPosition& ewp)
	    : Event(ewp), xPos(ewp.xPos), yPos(ewp.yPos) {}

	/**
	 *  @brief  Returns the new x coordinate for this window position change event.
	 *  @return The new x coordinate.
	 */
	int EventWindowPosition::getX() const { return xPos; }

	/**
	 *  @brief  Returns the new y coordinate for this window position change event.
	 *  @return The new y coordinate.
	 */
	int EventWindowPosition::getY() const { return yPos; }

	/**
	 *  @brief  Default constructor.
	 */
	EventWindowSize::EventWindowSize() : Event(AllWindowIDs, EventType::WINDOW_SIZE), width(0), height(0) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  w The new width.
	 *  @param  h The new height.
	 */
	EventWindowSize::EventWindowSize(const WindowID id, const int w, const int h)
	    : Event(id, EventType::WINDOW_SIZE), width(w), height(h) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ews The EventWindowPosition to copy.
	 */
	EventWindowSize::EventWindowSize(const EventWindowSize& ews) : Event(ews), width(ews.width), height(ews.height) {}

	/**
	 *  @brief  Returns the new width of this window size change event.
	 *  @return The new width coordinate.
	 */
	int EventWindowSize::getWidth() const { return width; }

	/**
	 *  @brief  Returns the new height of this window size change event.
	 *  @return The new height coordinate.
	 */
	int EventWindowSize::getHeight() const { return height; }

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 */
	EventWindowClose::EventWindowClose(const WindowID id) : Event(id, EventType::WINDOW_CLOSE) {}

	/**
	 *  @brief  Default constructor.
	 */
	EventWindowClose::EventWindowClose() : Event(AllWindowIDs, EventType::WINDOW_CLOSE) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ewc The EventWindowClose to copy.
	 */
	EventWindowClose::EventWindowClose(const EventWindowClose& ewc) : Event(ewc) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 */
	EventWindowRefresh::EventWindowRefresh(const WindowID id) : Event(id, EventType::WINDOW_REFRESH) {}

	/**
	 *  @brief  Default constructor.
	 */
	EventWindowRefresh::EventWindowRefresh() : Event(AllWindowIDs, EventType::WINDOW_REFRESH) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ewr The EventWindowRefresh to copy.
	 */
	EventWindowRefresh::EventWindowRefresh(const EventWindowRefresh& ewr) : Event(ewr) {}

	/**
	 *  @brief  Default constructor.
	 */
	EventWindowFocus::EventWindowFocus() : Event(AllWindowIDs, EventType::WINDOW_FOCUS), focus(false) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  f true for got focus, false for lost focus.
	 */
	EventWindowFocus::EventWindowFocus(const WindowID id, const bool f)
	    : Event(id, EventType::WINDOW_FOCUS), focus(f) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ewf The EventWindowFocus to copy.
	 */
	EventWindowFocus::EventWindowFocus(const EventWindowFocus& ewf) : Event(ewf), focus(ewf.focus) {}

	/**
	 *  @brief  The hasFocus method says whether this event corresponds to a got or lost focus.
	 *  @return true if the window has got focus, false if it has lost focus.
	 */
	bool EventWindowFocus::hasFocus() const { return focus; }

	/**
	 *  @brief  Default constructor.
	 */
	EventWindowMaximize::EventWindowMaximize() : Event(AllWindowIDs, EventType::WINDOW_MAXIMIZE), maximize(false) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  m true for maximizing, false for restoring.
	 */
	EventWindowMaximize::EventWindowMaximize(const WindowID id, const bool m)
	    : Event(id, EventType::WINDOW_MAXIMIZE), maximize(m) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ewm The EventWindowMaximize to copy.
	 */
	EventWindowMaximize::EventWindowMaximize(const EventWindowMaximize& ewm) : Event(ewm), maximize(ewm.maximize) {}

	/**
	 *  @brief  The toIconify method says whether this event corresponds to a iconify or restore request.
	 *  @return true if the window should be iconified, false if it should be restored.
	 */
	bool EventWindowMaximize::toMaximize() const { return maximize; }

	/**
	 *  @brief  Default constructor.
	 */
	EventWindowIconify::EventWindowIconify() : Event(AllWindowIDs, EventType::WINDOW_ICONIFY), iconify(false) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  i true for iconifying, false for restoring.
	 */
	EventWindowIconify::EventWindowIconify(const WindowID id, const bool i)
	    : Event(id, EventType::WINDOW_ICONIFY), iconify(i) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ewi The EventWindowIconify to copy.
	 */
	EventWindowIconify::EventWindowIconify(const EventWindowIconify& ewi) : Event(ewi), iconify(ewi.iconify) {}

	/**
	 *  @brief  The toIconify method says whether this event corresponds to a iconify or restore request.
	 *  @return true if the window should be iconified, false if it should be restored.
	 */
	bool EventWindowIconify::toIconify() const { return iconify; }

	/**
	 *  @brief  Default constructor.
	 */
	EventFrameBufferSize::EventFrameBufferSize()
	    : Event(AllWindowIDs, EventType::FRAMEBUFFERSIZE), width(0), height(0) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  w The new width.
	 *  @param  h The new height.
	 */
	EventFrameBufferSize::EventFrameBufferSize(const WindowID id, const int w, const int h)
	    : Event(id, EventType::FRAMEBUFFERSIZE), width(w), height(h) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ews The EventFrameBufferSize to copy.
	 */
	EventFrameBufferSize::EventFrameBufferSize(const EventFrameBufferSize& efs)
	    : Event(efs), width(efs.width), height(efs.height) {}

	/**
	 *  @brief  Returns the new width of this window framebuffer size change event.
	 *  @return The new width coordinate.
	 */
	int EventFrameBufferSize::getWidth() const { return width; }

	/**
	 *  @brief  Returns the new height of this window framebuffer size change event.
	 *  @return The new height coordinate.
	 */
	int EventFrameBufferSize::getHeight() const { return height; }

	/**
	 *  @brief  Default constructor.
	 */
	EventContentScale::EventContentScale() : Event(AllWindowIDs, EventType::CONTENTSCALE), xScale(0), yScale(0) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  x The new scale in x.
	 *  @param  y The new scale in y.
	 */
	EventContentScale::EventContentScale(const WindowID id, const float x, const float y)
	    : Event(id, EventType::CONTENTSCALE), xScale(x), yScale(y) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  efs The EventContentScale to copy.
	 */
	EventContentScale::EventContentScale(const EventContentScale& ecs)
	    : Event(ecs), xScale(ecs.xScale), yScale(ecs.yScale) {}

	/**
	 *  @brief  Returns the new scale in x of this window content scale change event.
	 *  @return The new scale in x.
	 */
	float EventContentScale::getXScale() const { return xScale; }

	/**
	 *  @brief  Returns the new scale in y of this window content scale change event.
	 *  @return The new scale in y.
	 */
	float EventContentScale::getYScale() const { return yScale; }

	/**
	 *  @brief  Default constructor.
	 */
	EventChar::EventChar() : Event(AllWindowIDs, EventType::CHAR), character(0) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  c The new Unicode character.
	 */
	EventChar::EventChar(const WindowID id, const char32_t c) : Event(id, EventType::CHAR), character(c) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ec The EventChar to copy.
	 */
	EventChar::EventChar(const EventChar& ec) : Event(ec), character(ec.character) {}

	/**
	 *  @brief  Returns the new Unicode codepoint of this input character event.
	 *  @return The new input character.
	 */
	char32_t EventChar::getCharacter() const { return character; }

	/**
	 *  @brief  Default constructor.
	 */
	EventCharMod::EventCharMod()
	    : Event(AllWindowIDs, EventType::CHARMOD),
	      character(0),
	      modifierKeys(static_cast<ModifierKeyBaseType>(ModifierKeyType::MODIFIER_NONE)) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  c The new Unicode character.
	 *  @param  m The new modifier keys (as a bit set of ModifierKeyType values).
	 */
	EventCharMod::EventCharMod(const WindowID id, const char32_t c, const ModifierKeyBaseType m)
	    : Event(id, EventType::CHARMOD), character(c), modifierKeys(m) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ec The EventChar to copy.
	 */
	EventCharMod::EventCharMod(const EventCharMod& ecm)
	    : Event(ecm), character(ecm.character), modifierKeys(ecm.modifierKeys) {}

	/**
	 *  @brief  Returns the new Unicode codepoint of this input event.
	 *  @return The new input character.
	 */
	char32_t EventCharMod::getCharacter() const { return character; }

	/**
	 *  @brief  Returns the modifier keys of this input event.
	 *  @return The input modifier keys (as a bit set of ModifierKeyType values).
	 */
	ModifierKeyBaseType EventCharMod::getModifierKeys() const { return modifierKeys; }

	/**
	 *  @brief  Default constructor.
	 */
	EventCursorEnter::EventCursorEnter() : Event(AllWindowIDs, EventType::CURSOR_ENTER), enter(false) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  e true for entering, false for exiting.
	 */
	EventCursorEnter::EventCursorEnter(const WindowID id, const bool e)
	    : Event(id, EventType::CURSOR_ENTER), enter(e) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ece The EventCursorEnter to copy.
	 */
	EventCursorEnter::EventCursorEnter(const EventCursorEnter& ece) : Event(ece), enter(ece.enter) {}

	/**
	 *  @brief  The isEntering method says whether this event corresponds to an cursor entering or exiting event.
	 *  @return true if the cursor is entering, false if it is exiting.
	 */
	bool EventCursorEnter::isEntering() const { return enter; }

	/**
	 *  @brief  Default constructor.
	 */
	EventCursorPosition::EventCursorPosition() : Event(AllWindowIDs, EventType::CURSOR_POSITION), xPos(0), yPos(0) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  x The new x coordinate.
	 *  @param  y The new y coordinate.
	 */
	EventCursorPosition::EventCursorPosition(const WindowID id, const double x, const double y)
	    : Event(id, EventType::CURSOR_POSITION), xPos(x), yPos(y) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ecp The EventCursorPosition to copy.
	 */
	EventCursorPosition::EventCursorPosition(const EventCursorPosition& ecp)
	    : Event(ecp), xPos(ecp.xPos), yPos(ecp.yPos) {}

	/**
	 *  @brief  Returns the new x coordinate for this cursor position change event.
	 *  @return The new x coordinate.
	 */
	double EventCursorPosition::getX() const { return xPos; }

	/**
	 *  @brief  Returns the new y coordinate for this cursor position change event.
	 *  @return The new y coordinate.
	 */
	double EventCursorPosition::getY() const { return yPos; }

	/**
	 *  @brief  Default constructor.
	 */
	EventDrop::EventDrop() : Event(AllWindowIDs, EventType::DROP) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  ps The new Unicode string paths.
	 */
	EventDrop::EventDrop(const WindowID id, const std::list<std::string>& ps) : Event(id, EventType::DROP), paths(ps) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ed The EventDrop to copy.
	 */
	EventDrop::EventDrop(const EventDrop& ed) : Event(ed), paths(ed.paths) {}

	/**
	 *  @brief  Returns the new Unicode strings of this input drop event.
	 *  @return The new string paths input drop.
	 */
	const std::list<std::string>& EventDrop::getPaths() const { return paths; }

	/**
	 *  @brief  Default constructor.
	 */
	EventKey::EventKey()
	    : Event(AllWindowIDs, EventType::KEY),
	      key(KeyType::KEY_UNKNOWN),
	      scancode(0),
	      action(ActionType::NONE),
	      modifierKeys(static_cast<ModifierKeyBaseType>(ModifierKeyType::MODIFIER_NONE)) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  k The key identifier.
	 *  @param  s The system-specific scancode of this key.
	 *  @param  a The action (press, release or repeat).
	 *  @param  m The new modifier keys (as a bit set of ModifierKeyType values).
	 */
	EventKey::EventKey(const WindowID id,
	                   const KeyType k,
	                   const char32_t s,
	                   const ActionType a,
	                   const ModifierKeyBaseType m)
	    : Event(id, EventType::KEY), key(k), scancode(s), action(a), modifierKeys(m) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  ek The EventKey to copy.
	 */
	EventKey::EventKey(const EventKey& ek)
	    : Event(ek), key(ek.key), scancode(ek.scancode), action(ek.action), modifierKeys(ek.modifierKeys) {}

	/**
	 *  @brief  Returns the key identifier for this key event.
	 *  @return The key identifier.
	 */
	KeyType EventKey::getKey() const { return key; }

	/**
	 *  @brief  Returns the system-specific scancode for this key event.
	 *  @return The system-specific scancode of the key.
	 */
	char32_t EventKey::getScancode() const { return scancode; }

	/**
	 *  @brief  Returns the action (press, release or repeat) for this key event.
	 *  @return The action of the key.
	 */
	ActionType EventKey::getAction() const { return action; }

	/**
	 *  @brief  Returns the modifier keys of this key event.
	 *  @return The input modifier keys (as a bit set of ModifierKeyType values).
	 */
	ModifierKeyBaseType EventKey::getModifierKeys() const { return modifierKeys; }

	/**
	 *  @brief  Default constructor.
	 */
	EventMouseButton::EventMouseButton()
	    : Event(AllWindowIDs, EventType::MOUSE_BUTTON),
	      mouseButton(MouseButtonType::MOUSE_BUTTON_NONE),
	      action(ActionType::NONE),
	      modifierKeys(static_cast<ModifierKeyBaseType>(ModifierKeyType::MODIFIER_NONE)) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  mb The key identifier.
	 *  @param  a The action (press, release or repeat).
	 *  @param  m The new modifier keys (as a bit set of ModifierKeyType values).
	 */
	EventMouseButton::EventMouseButton(const WindowID id,
	                                   const MouseButtonType mb,
	                                   const ActionType a,
	                                   const ModifierKeyBaseType m)
	    : Event(id, EventType::MOUSE_BUTTON), mouseButton(mb), action(a), modifierKeys(m) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  emb The EventMouseButton to copy.
	 */
	EventMouseButton::EventMouseButton(const EventMouseButton& emb)
	    : Event(emb), mouseButton(emb.mouseButton), action(emb.action), modifierKeys(emb.modifierKeys) {}

	/**
	 *  @brief  Returns the button identifier for this mouse button event.
	 *  @return The button identifier.
	 */
	MouseButtonType EventMouseButton::getMouseButton() const { return mouseButton; }

	/**
	 *  @brief  Returns the action (press, release or repeat) for this mouse button event.
	 *  @return The action of the mouse button.
	 */
	ActionType EventMouseButton::getAction() const { return action; }

	/**
	 *  @brief  Returns the modifier keys of this mouse button event.
	 *  @return The input modifier keys (as a bit set of ModifierKeyType values).
	 */
	ModifierKeyBaseType EventMouseButton::getModifierKeys() const { return modifierKeys; }

	/**
	 *  @brief  Default constructor.
	 */
	EventScroll::EventScroll() : Event(AllWindowIDs, EventType::SCROLL), xOffset(0), yOffset(0) {}

	/**
	 *  @brief  Initializer constructor.
	 *  @param  id The ID of the window.
	 *  @param  x The x offset.
	 *  @param  y The y offset.
	 */
	EventScroll::EventScroll(const WindowID id, const double x, const double y)
	    : Event(id, EventType::SCROLL), xOffset(x), yOffset(y) {}

	/**
	 *  @brief  Copy constructor.
	 *  @param  es The EventScroll to copy.
	 */
	EventScroll::EventScroll(const EventScroll& es) : Event(es), xOffset(es.xOffset), yOffset(es.yOffset) {}

	/**
	 *  @brief  Returns the x offset for this scroll event.
	 *  @return The x offset.
	 */
	double EventScroll::getX() const { return xOffset; }

	/**
	 *  @brief  Returns the y offset for this scroll event.
	 *  @return The y offset.
	 */
	double EventScroll::getY() const { return yOffset; }

}
