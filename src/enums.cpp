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

#include <GLFWM/enums.hpp>

namespace glfwm {

	// overloading AND assignment for modifier key types
	ModifierKeyBaseType& operator&=(ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt) {
		mkbt &= static_cast<ModifierKeyBaseType>(mkt);
		return mkbt;
	}
	// overloading OR assignment for modifier key types
	ModifierKeyBaseType& operator|=(ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt) {
		mkbt |= static_cast<ModifierKeyBaseType>(mkt);
		return mkbt;
	}
	// overloading XOR assignment for modifier key types
	ModifierKeyBaseType& operator^=(ModifierKeyBaseType& mkbt, const ModifierKeyType& mkt) {
		mkbt ^= static_cast<ModifierKeyBaseType>(mkt);
		return mkbt;
	}

	// overloading AND assignment for event types
	EventBaseType& operator&=(EventBaseType& ebt, const EventType& et) {
		ebt &= static_cast<EventBaseType>(et);
		return ebt;
	}
	// overloading OR assignment for event types
	EventBaseType& operator|=(EventBaseType& ebt, const EventType& et) {
		ebt |= static_cast<EventBaseType>(et);
		return ebt;
	}
	// overloading XOR assignment for event types
	EventBaseType& operator^=(EventBaseType& ebt, const EventType& et) {
		ebt ^= static_cast<EventBaseType>(et);
		return ebt;
	}

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
	std::string getKeyName(const KeyType k, const char32_t s) {
		return glfwGetKeyName(static_cast<KeyBaseType>(k), static_cast<int>(s));
	}
}
