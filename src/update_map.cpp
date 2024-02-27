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

#include <GLFWM/update_map.hpp>

namespace glfwm {

	std::unordered_map<WindowGroupID, std::unordered_set<WindowID>> UpdateMap::groups_windows;

#ifndef NO_MULTITHREADING
	std::mutex UpdateMap::globalMutex;
#endif

	/**
	 *  @brief  The notify static method is used to wake up the WindowManager and send a signal to a Window or
	 * WindowGroup target to update itself.
	 *  @param gID The ID of the WindowGroup to update. It is ignored unless wID is WholeGroupWindowIDs or AllWindowIDs,
	 * otherwise it must be a valid ID.
	 *  @param wID The ID of the Window to update, or WholeGroupWindowIDs for an entire group, or AllWindowIDs for all
	 * Windows.
	 */
	void UpdateMap::notify(const WindowGroupID gID, const WindowID wID) {
		setToUpdate(gID, wID);
		glfwPostEmptyEvent();
	}

	/**
	 *  @brief  The setToUpdate static method is used to collect a Window or WindowGroup target to update itself.
	 *  @param gID The ID of the WindowGroup to update. It is ignored unless wID is WholeGroupWindowIDs or AllWindowIDs,
	 * otherwise it must be a valid ID.
	 *  @param wID The ID of the Window to update, or WholeGroupWindowIDs for an entire group, or AllWindowIDs for all
	 * Windows.
	 */
	void UpdateMap::setToUpdate(const WindowGroupID gID, const WindowID wID) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::mutex> lock(globalMutex);
#endif
		groups_windows[gID].insert(wID);
	}

	/**
	 *  @brief  The popGroup static method is used by WindowManager to get the next group to wake up.
	 *  @param  gID The output WindowGrupID of the group to update.
	 *  @param  wIDs The output WindowIDs of the group to update.
	 */
	void UpdateMap::popGroup(WindowGroupID& gID, std::unordered_set<WindowID>& wIDs) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::mutex> lock(globalMutex);
#endif
		gID = NoWindowGroupID;
		wIDs.clear();
		if (!groups_windows.empty()) {
			std::unordered_map<WindowGroupID, std::unordered_set<WindowID>>::iterator gIt = groups_windows.find(
			    AllWindowGroupIDs);
			if (gIt == groups_windows.end())
				gIt = groups_windows.find(AnyWindowGroupID);
			if (gIt != groups_windows.end()) {
				gID = AllWindowGroupIDs;
				std::unordered_set<WindowID>::iterator wIt = gIt->second.find(AllWindowIDs);
				if (wIt != gIt->second.end()) {
					wIDs.insert(AllWindowIDs);
					groups_windows.clear();
					return;
				}
			}
			gID = groups_windows.begin()->first;
			wIDs = std::move(groups_windows.begin()->second);
			groups_windows.erase(groups_windows.begin());
		}
	}

	/**
	 *  @brief  The empty static methos says if the queue is currently empty.
	 *  @return true if empty, false otherwise.
	 */
	bool UpdateMap::empty() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::mutex> lock(globalMutex);
#endif
		return groups_windows.empty();
	}

}
