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

#ifndef GLFWM_ENUMS_HPP
#define GLFWM_ENUMS_HPP

#include <GLFWM/utility.hpp>

namespace glfwm {

	using WindowID = size_t;
	constexpr WindowID LastWindowID = std::numeric_limits<WindowID>::max() - 2;
	constexpr WindowID WholeGroupWindowIDs = std::numeric_limits<WindowID>::max() - 1;
	constexpr WindowID AllWindowIDs = std::numeric_limits<WindowID>::max();

	using WindowGroupID = size_t;
	constexpr WindowGroupID LastWindowGroupID = std::numeric_limits<WindowGroupID>::max() - 3;
	constexpr WindowGroupID NoWindowGroupID = std::numeric_limits<WindowGroupID>::max() - 2;
	constexpr WindowGroupID AnyWindowGroupID = std::numeric_limits<WindowGroupID>::max() - 1;
	constexpr WindowGroupID AllWindowGroupIDs = std::numeric_limits<WindowGroupID>::max();

	/**
	 *  @brief  InputModeBaseType is the base type used to identify the GLFW input modes.
	 */
	using InputModeBaseType = int;

	/**
	 *  @brief  The InputModeType represents the GLFW input modes.
	 */
	enum class InputModeType : InputModeBaseType {
		CURSOR = GLFW_CURSOR,
		STICKY_KEYS = GLFW_STICKY_KEYS,
		STICKY_MOUSE_BUTTONS = GLFW_STICKY_MOUSE_BUTTONS
	};

	/**
	 *  @brief  InputModeValueBaseType is the base type used to identify the values for GLFW input modes.
	 */
	using InputModeValueBaseType = int;

	/**
	 *  @brief  The InputModeValueType represents the GLFW input modes' values.
	 */
	enum class InputModeValueType : InputModeValueBaseType {
		CURSOR_NORMAL = GLFW_CURSOR_NORMAL,
		CURSOR_HIDDEN = GLFW_CURSOR_HIDDEN,
		CURSOR_DISABLED = GLFW_CURSOR_DISABLED,
		STICKY_TRUE = GL_TRUE,
		STICKY_FALSE = GL_FALSE
	};

	/**
	 *  @brief  ModifierKeyBaseType is the base type used to store a bit set for modifier key types.
	 */
	using ModifierKeyBaseType = int;

	/**
	 *  @brief  The ModifierKeyType enum represents the modifier key types.
	 */
	enum class ModifierKeyType : ModifierKeyBaseType {
		MODIFIER_NONE = 0x0000,
		MODIFIER_SHIFT = GLFW_MOD_SHIFT,
		MODIFIER_CTRL = GLFW_MOD_CONTROL,
		MODIFIER_ALT = GLFW_MOD_ALT,
		MODIFIER_SUPER = GLFW_MOD_SUPER
	};

	// overloading NOT for modifier key types
	inline constexpr ModifierKeyBaseType operator~(const ModifierKeyType& mkt) {
		return ~static_cast<ModifierKeyBaseType>(mkt);
	}
	// overloading AND for modifier key types
	inline constexpr ModifierKeyBaseType operator&(const ModifierKeyType& lmkt, const ModifierKeyType& rmkt) {
		return static_cast<ModifierKeyBaseType>(lmkt) & static_cast<ModifierKeyBaseType>(rmkt);
	}
	inline constexpr ModifierKeyBaseType operator&(const ModifierKeyType& mkt, const ModifierKeyBaseType& mkbt) {
		return static_cast<ModifierKeyBaseType>(mkt) & mkbt;
	}
	inline constexpr ModifierKeyBaseType operator&(const ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt) {
		return mkbt & static_cast<ModifierKeyBaseType>(mkt);
	}
	// overloading OR for modifier key types
	inline constexpr ModifierKeyBaseType operator|(const ModifierKeyType& lmkt, const ModifierKeyType& rmkt) {
		return static_cast<ModifierKeyBaseType>(lmkt) | static_cast<ModifierKeyBaseType>(rmkt);
	}
	inline constexpr ModifierKeyBaseType operator|(const ModifierKeyType& mkt, const ModifierKeyBaseType& mkbt) {
		return static_cast<ModifierKeyBaseType>(mkt) | mkbt;
	}
	inline constexpr ModifierKeyBaseType operator|(const ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt) {
		return mkbt | static_cast<ModifierKeyBaseType>(mkt);
	}
	// overloading XOR for modifier key types
	inline constexpr ModifierKeyBaseType operator^(const ModifierKeyType& lmkt, const ModifierKeyType& rmkt) {
		return static_cast<ModifierKeyBaseType>(lmkt) ^ static_cast<ModifierKeyBaseType>(rmkt);
	}
	inline constexpr ModifierKeyBaseType operator^(const ModifierKeyType& mkt, const ModifierKeyBaseType& mkbt) {
		return static_cast<ModifierKeyBaseType>(mkt) ^ mkbt;
	}
	inline constexpr ModifierKeyBaseType operator^(const ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt) {
		return mkbt ^ static_cast<ModifierKeyBaseType>(mkt);
	}
	// overloading AND assignment for modifier key types
	ModifierKeyBaseType& operator&=(ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt);
	// overloading OR assignment for modifier key types
	ModifierKeyBaseType& operator|=(ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt);
	// overloading XOR assignment for modifier key types
	ModifierKeyBaseType& operator^=(ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt);

	/**
	 *  @brief  The MouseButtonBaseType is the base type used to store a bit set for mouse button types.
	 */
	using MouseButtonBaseType = int;

	/**
	 *  @brief  The MouseButtonType enum represents the mouse button types.
	 */
	enum class MouseButtonType : MouseButtonBaseType {
		MOUSE_BUTTON_NONE = -1,
		MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1,
		MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2,
		MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3,
		MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4,
		MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5,
		MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6,
		MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7,
		MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8,
		MOUSE_BUTTON_LAST = GLFW_MOUSE_BUTTON_LAST,
		MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
	};

	/**
	 *  @brief  The KeyBaseType is the base type used to store a bit set for key types.
	 */
	using KeyBaseType = int;

	/**
	 *  @brief  The KeyType enum represents the key types.
	 */
	enum class KeyType : KeyBaseType {
		KEY_UNKNOWN = GLFW_KEY_UNKNOWN,
		KEY_SPACE = GLFW_KEY_SPACE,
		KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE,
		KEY_COMMA = GLFW_KEY_COMMA,
		KEY_MINUS = GLFW_KEY_MINUS,
		KEY_PERIOD = GLFW_KEY_PERIOD,
		KEY_SLASH = GLFW_KEY_SLASH,
		KEY_0 = GLFW_KEY_0,
		KEY_1 = GLFW_KEY_1,
		KEY_2 = GLFW_KEY_2,
		KEY_3 = GLFW_KEY_3,
		KEY_4 = GLFW_KEY_4,
		KEY_5 = GLFW_KEY_5,
		KEY_6 = GLFW_KEY_6,
		KEY_7 = GLFW_KEY_7,
		KEY_8 = GLFW_KEY_8,
		KEY_9 = GLFW_KEY_9,
		KEY_SEMICOLON = GLFW_KEY_SEMICOLON,
		KEY_EQUAL = GLFW_KEY_EQUAL,
		KEY_A = GLFW_KEY_A,
		KEY_B = GLFW_KEY_B,
		KEY_C = GLFW_KEY_C,
		KEY_D = GLFW_KEY_D,
		KEY_E = GLFW_KEY_E,
		KEY_F = GLFW_KEY_F,
		KEY_G = GLFW_KEY_G,
		KEY_H = GLFW_KEY_H,
		KEY_I = GLFW_KEY_I,
		KEY_J = GLFW_KEY_J,
		KEY_K = GLFW_KEY_K,
		KEY_L = GLFW_KEY_L,
		KEY_M = GLFW_KEY_M,
		KEY_N = GLFW_KEY_N,
		KEY_O = GLFW_KEY_O,
		KEY_P = GLFW_KEY_P,
		KEY_Q = GLFW_KEY_Q,
		KEY_R = GLFW_KEY_R,
		KEY_S = GLFW_KEY_S,
		KEY_T = GLFW_KEY_T,
		KEY_U = GLFW_KEY_U,
		KEY_V = GLFW_KEY_V,
		KEY_W = GLFW_KEY_W,
		KEY_X = GLFW_KEY_X,
		KEY_Y = GLFW_KEY_Y,
		KEY_Z = GLFW_KEY_Z,
		KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
		KEY_BACKSLASH = GLFW_KEY_BACKSLASH,
		KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
		KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
		KEY_WORLD_1 = GLFW_KEY_WORLD_1,
		KEY_WORLD_2 = GLFW_KEY_WORLD_2,
		KEY_ESCAPE = GLFW_KEY_ESCAPE,
		KEY_ENTER = GLFW_KEY_ENTER,
		KEY_TAB = GLFW_KEY_TAB,
		KEY_BACKSPACE = GLFW_KEY_BACKSPACE,
		KEY_INSERT = GLFW_KEY_INSERT,
		KEY_DELETE = GLFW_KEY_DELETE,
		KEY_RIGHT = GLFW_KEY_RIGHT,
		KEY_LEFT = GLFW_KEY_LEFT,
		KEY_DOWN = GLFW_KEY_DOWN,
		KEY_UP = GLFW_KEY_UP,
		KEY_PAGE_UP = GLFW_KEY_PAGE_UP,
		KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
		KEY_HOME = GLFW_KEY_HOME,
		KEY_END = GLFW_KEY_END,
		KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
		KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
		KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK,
		KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
		KEY_PAUSE = GLFW_KEY_PAUSE,
		KEY_F1 = GLFW_KEY_F1,
		KEY_F2 = GLFW_KEY_F2,
		KEY_F3 = GLFW_KEY_F3,
		KEY_F4 = GLFW_KEY_F4,
		KEY_F5 = GLFW_KEY_F5,
		KEY_F6 = GLFW_KEY_F6,
		KEY_F7 = GLFW_KEY_F7,
		KEY_F8 = GLFW_KEY_F8,
		KEY_F9 = GLFW_KEY_F9,
		KEY_F10 = GLFW_KEY_F10,
		KEY_F11 = GLFW_KEY_F11,
		KEY_F12 = GLFW_KEY_F12,
		KEY_F13 = GLFW_KEY_F13,
		KEY_F14 = GLFW_KEY_F14,
		KEY_F15 = GLFW_KEY_F15,
		KEY_F16 = GLFW_KEY_F16,
		KEY_F17 = GLFW_KEY_F17,
		KEY_F18 = GLFW_KEY_F18,
		KEY_F19 = GLFW_KEY_F19,
		KEY_F20 = GLFW_KEY_F20,
		KEY_F21 = GLFW_KEY_F21,
		KEY_F22 = GLFW_KEY_F22,
		KEY_F23 = GLFW_KEY_F23,
		KEY_F24 = GLFW_KEY_F24,
		KEY_F25 = GLFW_KEY_F25,
		KEY_PAD_0 = GLFW_KEY_KP_0,
		KEY_PAD_1 = GLFW_KEY_KP_1,
		KEY_PAD_2 = GLFW_KEY_KP_2,
		KEY_PAD_3 = GLFW_KEY_KP_3,
		KEY_PAD_4 = GLFW_KEY_KP_4,
		KEY_PAD_5 = GLFW_KEY_KP_5,
		KEY_PAD_6 = GLFW_KEY_KP_6,
		KEY_PAD_7 = GLFW_KEY_KP_7,
		KEY_PAD_8 = GLFW_KEY_KP_8,
		KEY_PAD_9 = GLFW_KEY_KP_9,
		KEY_PAD_DECIMAL = GLFW_KEY_KP_DECIMAL,
		KEY_PAD_DIVIDE = GLFW_KEY_KP_DIVIDE,
		KEY_PAD_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
		KEY_PAD_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
		KEY_PAD_ADD = GLFW_KEY_KP_ADD,
		KEY_PAD_ENTER = GLFW_KEY_KP_ENTER,
		KEY_PAD_EQUAL = GLFW_KEY_KP_EQUAL,
		KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
		KEY_LEFT_CTRL = GLFW_KEY_LEFT_CONTROL,
		KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT,
		KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
		KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
		KEY_RIGHT_CTRL = GLFW_KEY_RIGHT_CONTROL,
		KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
		KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
		KEY_MENU = GLFW_KEY_MENU,
		KEY_LAST = GLFW_KEY_LAST
	};

	/**
	 *  @brief  The getKeyName function returns the localized name of the specified printable key.
	 *          This is intended for displaying key bindings to the user.
	 *
	 *  If the key is KEY_UNKNOWN, the scancode is used instead, otherwise the scancode is ignored.
	 *  If a non-printable key or (if the key is KEY_UNKNOWN) a scancode that maps to a non-printable
	 *  key is specified, this function returns an empty string.
	 *
	 *  @param k The key to query.
	 *  @param s The scancode of the key to query.
	 *  @return The localized name of the key.
	 *  @note   This may only be called from the main thread.
	 */
	std::string getKeyName(const KeyType k, const char32_t s);

	/**
	 *  @brief  The ActionType enum represents the action types of an input event.
	 */
	enum class ActionType : int { NONE = -1, PRESS = GLFW_PRESS, RELEASE = GLFW_RELEASE, REPEAT = GLFW_REPEAT };

	/**
	 *  @brief  EventBaseType is the base type used to store a bit set for event types.
	 */
	using EventBaseType = unsigned int;

	/**
	 *  @brief  The EventType enum represents the event types.
	 */
	enum class EventType : EventBaseType {
		EMPTY = 0x00000,
		// window events
		WINDOW_POSITION = 0x00001,
		WINDOW_SIZE = 0x00002,
		WINDOW_CLOSE = 0x00004,
		WINDOW_REFRESH = 0x00008,
		WINDOW_FOCUS = 0x00010,
		WINDOW_ICONIFY = 0x00020,
		WINDOW_MAXIMIZE = 0x00040,
		FRAMEBUFFERSIZE = 0x00080,
		CONTENTSCALE = 0x00100,
		// input events
		CHAR = 0x00200,
		CHARMOD = 0x00400,
		CURSOR_ENTER = 0x00800,
		CURSOR_POSITION = 0x01000,
		DROP = 0x02000,
		KEY = 0x04000,
		MOUSE_BUTTON = 0x08000,
		SCROLL = 0x10000
	};

	// overloading NOT for event types
	inline constexpr EventBaseType operator~(const EventType& et) { return ~static_cast<EventBaseType>(et); }
	// overloading AND for event types
	inline constexpr EventBaseType operator&(const EventType& let, const EventType& ret) {
		return static_cast<EventBaseType>(let) & static_cast<EventBaseType>(ret);
	}
	inline constexpr EventBaseType operator&(const EventType& et, const EventBaseType& ebt) {
		return static_cast<EventBaseType>(et) & ebt;
	}
	inline constexpr EventBaseType operator&(const EventBaseType& ebt, const EventType& et) {
		return ebt & static_cast<EventBaseType>(et);
	}
	// overloading OR for event types
	inline constexpr EventBaseType operator|(const EventType& let, const EventType& ret) {
		return static_cast<EventBaseType>(let) | static_cast<EventBaseType>(ret);
	}
	inline constexpr EventBaseType operator|(const EventType& et, const EventBaseType& ebt) {
		return static_cast<EventBaseType>(et) | ebt;
	}
	inline constexpr EventBaseType operator|(const EventBaseType& ebt, const EventType& et) {
		return ebt | static_cast<EventBaseType>(et);
	}
	// overloading XOR for event types
	inline constexpr EventBaseType operator^(const EventType& let, const EventType& ret) {
		return static_cast<EventBaseType>(let) ^ static_cast<EventBaseType>(ret);
	}
	inline constexpr EventBaseType operator^(const EventType& et, const EventBaseType& ebt) {
		return static_cast<EventBaseType>(et) ^ ebt;
	}
	inline constexpr EventBaseType operator^(const EventBaseType& ebt, const EventType& et) {
		return ebt ^ static_cast<EventBaseType>(et);
	}
	// overloading AND assignment for event types
	EventBaseType& operator&=(EventBaseType& ebt, const EventType& et);
	// overloading OR assignment for event types
	EventBaseType& operator|=(EventBaseType& ebt, const EventType& et);
	// overloading XOR assignment for event types
	EventBaseType& operator^=(EventBaseType& ebt, const EventType& et);

	constexpr EventBaseType allEventTypes = EventType::WINDOW_POSITION | EventType::WINDOW_SIZE
	    | EventType::WINDOW_CLOSE | EventType::WINDOW_REFRESH | EventType::WINDOW_FOCUS | EventType::WINDOW_ICONIFY
	    | EventType::FRAMEBUFFERSIZE | EventType::CONTENTSCALE | EventType::CHAR | EventType::CHARMOD
	    | EventType::CURSOR_ENTER | EventType::CURSOR_POSITION | EventType::DROP | EventType::KEY
	    | EventType::MOUSE_BUTTON | EventType::SCROLL;

}

#endif
