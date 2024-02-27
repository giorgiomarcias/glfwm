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

#ifndef GLFWM_UPDATE_MAP_HPP
#define GLFWM_UPDATE_MAP_HPP

#include <GLFWM/enums.hpp>

namespace glfwm {

	/**
	 *  @brief  The UpdateMap class represents a global map specifically designed for the signaling
	 *          between WindowManager and WindowGroups.
	 */
	class UpdateMap {
	  public:
		/**
		 *  @brief  The notify static method is used to wake up the WindowManager and send a signal to a Window or
		 * WindowGroup target to update itself.
		 *  @param gID The ID of the WindowGroup to update. It is ignored unless wID is WholeGroupWindowIDs or
		 * AllWindowIDs, otherwise it must be a valid ID.
		 *  @param wID The ID of the Window to update, or WholeGroupWindowIDs for an entire group, or AllWindowIDs for
		 * all Windows.
		 */
		static void notify(const WindowGroupID gID = AnyWindowGroupID, const WindowID wID = AllWindowIDs);

	  private:
		friend class WindowManager;

		/**
		 *  @brief  The setToUpdate static method is used to collect a Window or WindowGroup target to update itself.
		 *  @param gID The ID of the WindowGroup to update. It is ignored unless wID is WholeGroupWindowIDs or
		 * AllWindowIDs, otherwise it must be a valid ID.
		 *  @param wID The ID of the Window to update, or WholeGroupWindowIDs for an entire group, or AllWindowIDs for
		 * all Windows.
		 */
		static void setToUpdate(const WindowGroupID gID = AnyWindowGroupID, const WindowID wID = AllWindowIDs);

		/**
		 *  @brief  The popGroup static method is used by WindowManager to get the next group to wake up.
		 *  @param  gID The output WindowGrupID of the group to update.
		 *  @param  wIDs The output WindowIDs of the group to update.
		 */
		static void popGroup(WindowGroupID& gID, std::unordered_set<WindowID>& wIDs);

		/**
		 *  @brief  The empty static methos says if the queue is currently empty.
		 *  @return true if empty, false otherwise.
		 */
		static bool empty();

		/**
		 *  @brief  The queue of Events.
		 */
		static std::unordered_map<WindowGroupID, std::unordered_set<WindowID>> groups_windows;

#ifndef NO_MULTITHREADING
		/**
		 *  @brief  Mutex used to guarantee exclusive access to the elements in the queue.
		 */
		static std::mutex globalMutex;
#endif
	};
}

#endif
