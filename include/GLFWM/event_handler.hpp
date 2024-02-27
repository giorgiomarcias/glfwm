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

#ifndef GLFWM_EVENT_HANDLER_HPP
#define GLFWM_EVENT_HANDLER_HPP

#include <GLFWM/event.hpp>

namespace glfwm {

	/// The EventHandler class represents an entity able to handle an Event. Inherit this class and bind its objects to
	/// a window to react to events with specialized actions.
	class EventHandler {
	  public:
		virtual ~EventHandler() = default;

		/**
		 *    @brief  The getHandledEventTypes method returns a bitwise OR combination of the EventTypes
		 *            this object is able to handle.
		 *            Any Window will use this to skip handlers that can not handle a given event type.
		 */
		virtual EventBaseType getHandledEventTypes() const = 0;

		/**
		 *    @brief  The handle method receives an event and react to it.
		 *    @param e The event received.
		 *    @return true if the event has been handled, false otherwise.
		 */
		virtual bool handle(const EventPointer& e) = 0;
	};

	/**
	 *    @brief  A smart pointer to a EventHandler object. To bind A EventHandler to a Window, a smart pointer must be
	 * used.
	 */
	using EventHandlerPointer = std::shared_ptr<EventHandler>;
}

#endif
