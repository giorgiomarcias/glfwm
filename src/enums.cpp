// Copyright (c) 2015-2016 Giorgio Marcias
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

#include "enums.hpp"

namespace glfwm {

    // overloading AND assignment for modifier key types
    ModifierKeyBaseType & operator &= (ModifierKeyBaseType &mkbt, const ModifierKeyType &mkt) {
        mkbt &= static_cast<ModifierKeyBaseType>(mkt);
        return mkbt;
    }
    // overloading OR assignment for modifier key types
    ModifierKeyBaseType & operator |= (ModifierKeyBaseType &mkbt, const ModifierKeyType &mkt) {
        mkbt |= static_cast<ModifierKeyBaseType>(mkt);
        return mkbt;
    }
    // overloading XOR assignment for modifier key types
    ModifierKeyBaseType & operator ^= (ModifierKeyBaseType &mkbt, const ModifierKeyType &mkt) {
        mkbt ^= static_cast<ModifierKeyBaseType>(mkt);
        return mkbt;
    }



    // overloading AND assignment for event types
    EventBaseType & operator &= (EventBaseType &ebt, const EventType &et) {
        ebt &= static_cast<EventBaseType>(et);
        return ebt;
    }
    // overloading OR assignment for event types
    EventBaseType & operator |= (EventBaseType &ebt, const EventType &et) {
        ebt |= static_cast<EventBaseType>(et);
        return ebt;
    }
    // overloading XOR assignment for event types
    EventBaseType & operator ^= (EventBaseType &ebt, const EventType &et) {
        ebt ^= static_cast<EventBaseType>(et);
        return ebt;
    }

}
