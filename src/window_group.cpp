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

#include <GLFWM/window_group.hpp>

namespace glfwm {

	/**
	 *  @brief  Constructor. See newGroup for a reliable construction.
	 *  @param  id The univoque ID for thiw WindowGroup. See newGroupID.
	 */
	WindowGroup::WindowGroup(const WindowGroupID id)
	    : groupID(id)
#ifndef NO_MULTITHREADING
	      ,
	      doPoll(false),
	      doLoop(false)
#endif
	{
	}

	/**
	 *  @brief  Destructor.
	 */
	WindowGroup::~WindowGroup() { destroy(); }

	/**
	 *  @brief  Destroys this WindowGroup.
	 */
	void WindowGroup::destroy() {
#ifndef NO_MULTITHREADING
		stopAndWait();
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lockGlobal(globalMutex);
		std::lock_guard<std::mutex> lockLocal(mutex);
#endif
		for (auto id : attachedWindows)
			windowGroupMap[id] = NoWindowGroupID;
		attachedWindows.clear();
		std::deque<WindowGroupID>::iterator pos = std::lower_bound(freedWindowGroupIDs.begin(),
		                                                           freedWindowGroupIDs.end(),
		                                                           groupID);
		freedWindowGroupIDs.insert(pos, groupID);
	}

	/**
	 *  @brief  The getID method returns the ID of this group.
	 *  @return The ID of this group.
	 */
	WindowGroupID WindowGroup::getID() const { return groupID; }

	/**
	 *  @brief  The attachWindow method adds a Window to this group.
	 *  @param windowID The ID of the Window to attach.
	 *  @note   A Window can not belong to more than one group at the same time.
	 */
	void WindowGroup::attachWindow(const WindowID windowID) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lockGlobal(globalMutex);
		std::lock_guard<std::mutex> lockLocal(mutex);
#endif
		attachedWindows.insert(windowID);
		windowGroupMap[windowID] = groupID;
	}

	/**
	 *  @brief  The detachWindow method removes a Window from this group.
	 *  @param windowID The ID of the Window to detach.
	 */
	void WindowGroup::detachWindow(const WindowID windowID) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lockGlobal(globalMutex);
		std::lock_guard<std::mutex> lockLocal(mutex);
#endif
		if (attachedWindows.erase(windowID) > 0)
			windowGroupMap[windowID] = NoWindowGroupID;
	}

	/**
	 *  @brief  The empty method returns true if there are not Windows attached to this WindowGroup.
	 *  @return true if no Window is attached to this group, false otherwise.
	 */
	bool WindowGroup::empty() const {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::mutex> lock(mutex);
#endif
		return attachedWindows.empty();
	}

#ifndef NO_MULTITHREADING
	/**
	 *  @brief  The setPoll method sets this group's event processing to POLL or WAIT for events.
	 *  @param poll true for polling, false for waiting.
	 */
	void WindowGroup::setPoll(const bool poll) {
		bool wasWaiting = !doPoll;
		doPoll = poll;
		if (wasWaiting && poll)
			conditonVariable.notify_one();
	}

	/**
	 *  @brief  The isPolling method return if this group's event processing is currently in POLL or WAIT.
	 *  @return The current event processing method: true for POLL, false for WAIT.
	 */
	bool WindowGroup::isPolling() const { return doPoll; }

	/**
	 *  @brief  The runLoopConcurrently method starts the execution of this group loop in another thread, if not yet
	 * started.
	 *  @note   Use notifyEvents, stop, isRunningConcurrently and join to sinchronize.
	 */
	void WindowGroup::runLoopConcurrently() {
		std::lock_guard<std::mutex> lock(joinMutex);
		if (threadOfLoop.joinable()) {
			doLoop = true;
			threadOfLoop = std::thread(&WindowGroup::concurrentLoop, this);
		}
	}

	/**
	 *  @brief  The isRunningConcurrently method says if the loop is running on another thread.
	 *  @return true if running concurrently, false otherwise.
	 */
	bool WindowGroup::isRunningConcurrently() const {
		std::lock_guard<std::mutex> lock(joinMutex);
		return threadOfLoop.joinable();
	}

	/**
	 *  @brief  The stop method breaks the execution of the loop running on another thread.
	 *  @note   This does not sinchronize, just tells the thread to stop and returns immediately. See stopAndWait if you
	 * need to sinchronize.
	 */
	void WindowGroup::stop() {
		doLoop = false;
		conditonVariable.notify_one();
	}

	/**
	 *  @brief  The stopAndWait method breaks the executiotn of the loop running on another thread and sinchronizes with
	 * its ending.
	 */
	void WindowGroup::stopAndWait() {
		std::lock_guard<std::mutex> lock(joinMutex);
		if (threadOfLoop.joinable()) {
			doLoop = false;
			conditonVariable.notify_one();
			threadOfLoop.join();
		}
	}

	/**
	 *  @brief  The concurrentLoop method is the function to be executed on another thread and that represents a loop of
	 * event processing and drawing.
	 */
	void WindowGroup::concurrentLoop() {
		while (doLoop) {
			waitEvents();
			updateWindows();
		}
	}

	/**
	 *  @brief  The waitEvents method puts the current thread to sleep if the Event queue is empty.
	 */
	void WindowGroup::waitEvents() {
		std::unique_lock<std::mutex> lock(mutex);
		conditonVariable.wait(lock, [this]() -> bool { return !doLoop || doPoll || !windowsToUpdate.empty(); });
	}
#endif

	/**
	 *  @brief  The setWindowToUpdate method adds the window with ID wID to be updated.
	 *  @param wID The ID of the Window to update.
	 */
	void WindowGroup::setWindowToUpdate(const WindowID wID) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::mutex> lock(mutex);
#endif
		windowsToUpdate.insert(wID);
	}

	/**
	 *  @brief  The process method makes this group draw the windows in windowsToUpdate or all the attached windows (if
	 * doPoll), directly or indirectly (if running concurrently).
	 */
	void WindowGroup::process() {
#ifndef NO_MULTITHREADING
		if (isRunningConcurrently()) {
			if (!doPoll)
				conditonVariable.notify_one();
			return;
		}
#endif
		updateWindows();
	}

	/**
	 *  @brief  The updateWindows method calls the draw method of the Windows in windowsToUpdate.
	 */
	void WindowGroup::updateWindows() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::mutex> lock(mutex);
#endif
		WindowPointer w;
		if (
#ifndef NO_MULTITHREADING
		    doPoll ||
#endif
		    windowsToUpdate.find(WholeGroupWindowIDs) != windowsToUpdate.end()
		    || windowsToUpdate.find(AllWindowIDs) != windowsToUpdate.end())
			for (auto id : attachedWindows) {
				w = Window::getWindow(id);
				w->makeContextCurrent();
				w->draw();
				w->swapBuffers();
				w->doneCurrentContext();
			}
		else
			for (auto id : windowsToUpdate)
				if (attachedWindows.find(id) != attachedWindows.end()) {
					w = Window::getWindow(id);
					w->makeContextCurrent();
					w->draw();
					w->swapBuffers();
					w->doneCurrentContext();
				} else {
					UpdateMap::notify(AnyWindowGroupID, id);
				}
		windowsToUpdate.clear();
	}

	// static stuff

	/**
	 *  @brief  The container of WindowGroups.
	 */
	std::deque<WindowGroupPointer> WindowGroup::windowGroups;

	/**
	 *  @brief  The container for reusing freed WindowGroupID. It is kept sorted.
	 */
	std::deque<WindowGroupID> WindowGroup::freedWindowGroupIDs;

	/**
	 *  @brief  The map between Windows and WindowGroups.
	 */
	WindowGroup::WindowGroupMap WindowGroup::windowGroupMap;

#ifndef NO_MULTITHREADING
	/**
	 *  @brief  Mutex used to guarantee correct concurrent management of static activities.
	 */
	std::recursive_mutex WindowGroup::globalMutex;
#endif

	/**
	 *  @brief  The newGroupID static method books a new or an old & freed ID for WindowGroups.
	 *  @return The booked WindowGroupID.
	 */
	WindowGroupID WindowGroup::newGroupID() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		WindowGroupID id = windowGroups.size();
		if (!freedWindowGroupIDs.empty()) {
			id = freedWindowGroupIDs.front();
			freedWindowGroupIDs.pop_front();
		}
		if (id >= windowGroups.size())
			windowGroups.resize(id + 1);
		return id;
	}

	/**
	 *  @brief  The newGroup static method allocates and creates a new WindowGroup, storing also information for later
	 * retrieval.
	 *  @return A pointer to the created WindowGroup.
	 */
	WindowGroupPointer WindowGroup::newGroup() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		WindowGroupID id = newGroupID();
		windowGroups[id] = std::make_shared<WindowGroup>(id);
		return windowGroups[id];
	}

	/**
	 *  @brief  The getGroup static method returns a pointer to the WindowGroup with the given ID.
	 *  @param id The ID of the WindowGrop to retrieve.
	 *  @return A pointer to the requested WindowGroup, if it exists, or a null pointer.
	 */
	WindowGroupPointer WindowGroup::getGroup(const WindowGroupID id) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		if (id < windowGroups.size())
			return windowGroups[id];
		return WindowGroupPointer(nullptr);
	}

	/**
	 *  @brief  The getWindowGroup static method returns the ID of the group the Window identified by id belongs to.
	 *  @param id The ID of the Window.
	 *  @return The ID of the group the Window identified by id belongs to, or NoWindowGroup.
	 */
	WindowGroupID WindowGroup::getWindowGroup(const WindowID id) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		WindowGroupMapIterator it = windowGroupMap.find(id);
		if (it != windowGroupMap.end())
			return it->second;
		return NoWindowGroupID;
	}

	/**
	 *  @brief  The getAllWindowGroupsIDs static method returns the set of all the WindowGroupIDs currently in use.
	 *  @param gIDs The set of WindowGroupIDs.
	 */
	void WindowGroup::getAllWindowGroupIDs(std::unordered_set<WindowGroupID>& gIDs) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		gIDs.clear();
		for (auto& g : windowGroups)
			if (g)
				gIDs.insert(g->groupID);
	}

	/**
	 *  @brief  The getAllUngroupedWindowIDs static method returns the set of all the WindowIDs that are not attached to
	 * any group.
	 *  @param wIDs The set of WindowIDs.
	 */
	void WindowGroup::getAllUngroupedWindowIDs(std::unordered_set<WindowID>& wIDs) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		Window::getAllWindowIDs(wIDs);
		for (auto& wg : windowGroupMap)
			wIDs.erase(wg.first);
	}

	/**
	 *  @brief  The deleteWindowGroup static method destroys and removes the WindowGroup at id.
	 *  @param id The ID of the WindowGroup to delete.
	 */
	void WindowGroup::deleteWindowGroup(const WindowGroupID id) {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		if (id < windowGroups.size() && windowGroups[id]) {
			windowGroups[id]->destroy();
			windowGroups[id].reset();
		}
	}

	/**
	 *  @brief  The deleteAllWindowGroups static method destroys and removes all WindowGroup created so far.
	 */
	void WindowGroup::deleteAllWindowGroups() {
#ifndef NO_MULTITHREADING
		// acquire ownership
		std::lock_guard<std::recursive_mutex> lock(globalMutex);
#endif
		for (auto& g : windowGroups)
			if (g)
				g->destroy();
		windowGroupMap.clear();
		freedWindowGroupIDs.clear();
		windowGroups.clear();
	}

}
