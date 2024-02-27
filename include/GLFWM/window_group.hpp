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

#ifndef GLFWM_WINDOW_GROUP_HPP
#define GLFWM_WINDOW_GROUP_HPP

#include <GLFWM/update_map.hpp>
#include <GLFWM/window.hpp>
#ifndef NO_MULTITHREADING
#include <atomic>
#endif

namespace glfwm {

	class WindowGroup;

	/**
	 *  @brief  The WindowGroupPointer is a smart pointer to a WindowGroup.
	 */
	using WindowGroupPointer = std::shared_ptr<WindowGroup>;

	/**
	 *  @brief  The WindowGroup class represents a group of Windows which can be managed together to be drawn, and even
	 * moved to another thread.
	 */
	class WindowGroup {

	  public:
		/**
		 *  @brief  Constructor. See newGroup for a reliable construction.
		 *  @param  id The univoque ID for thiw WindowGroup. See newGroupID.
		 */
		WindowGroup(const WindowGroupID id);

		/**
		 *  @brief  The copy constructor is deleted, i.e. a WindowGroup can not be copied.
		 *  @param  The WindowGroup to copy.
		 */
		WindowGroup(const WindowGroup&) = delete;

		/**
		 *  @brief  The move constructor is deleted, i.e. a WindowGroup can not be moved.
		 *  @param  The WindowGroup to move.
		 */
		WindowGroup(WindowGroup&&) = delete;

		/**
		 *  @brief  The copy operator is deleted, i.e. a WindowGroup can not be copied.
		 *  @param  The WindowGroup to copy.
		 *  @return A reference to this WindowGroup.
		 */
		WindowGroup& operator=(const WindowGroup&) = delete;

		/**
		 *  @brief  The move operator is deleted, i.e. a WindowGroup can not be moved.
		 *  @param  The WindowGroup to move.
		 *  @return A reference to this WindowGroup.
		 */
		WindowGroup& operator=(WindowGroup&&) = delete;

		/**
		 *  @brief  Destructor.
		 */
		~WindowGroup();

		/**
		 *  @brief  Destroys this WindowGroup.
		 */
		void destroy();

		/**
		 *  @brief  The getID method returns the ID of this group.
		 *  @return The ID of this group.
		 */
		WindowGroupID getID() const;

		/**
		 *  @brief  The attachWindow method adds a Window to this group.
		 *  @param windowID The ID of the Window to attach.
		 *  @note   A Window can not belong to more than one group at the same time.
		 */
		void attachWindow(const WindowID windowID);

		/**
		 *  @brief  The detachWindow method removes a Window from this group.
		 *  @param windowID The ID of the Window to detach.
		 */
		void detachWindow(const WindowID windowID);

		/**
		 *  @brief  The empty method returns true if there are not Windows attached to this WindowGroup.
		 *  @return true if no Window is attached to this group, false otherwise.
		 */
		bool empty() const;

#ifndef NO_MULTITHREADING
		/**
		 *  @brief  The setPoll method sets this group's event processing to POLL or WAIT for events.
		 *  @param poll true for polling, false for waiting.
		 */
		void setPoll(const bool poll);

		/**
		 *  @brief  The isPolling method return if this group's event processing is currently in POLL or WAIT.
		 *  @return The current event processing method: true for POLL, false for WAIT.
		 */
		bool isPolling() const;

		/**
		 *  @brief  The runLoopConcurrently method starts the execution of this group loop in another thread, if not yet
		 * started.
		 *  @note   Use notifyEvents, stop, isRunningConcurrently and join to sinchronize.
		 */
		void runLoopConcurrently();

		/**
		 *  @brief  The isRunningConcurrently method says if the loop is running on another thread.
		 *  @return true if running concurrently, false otherwise.
		 */
		bool isRunningConcurrently() const;

		/**
		 *  @brief  The stop method breaks the execution of the loop running on another thread.
		 *  @note   This does not sinchronize, just tells the thread to stop and returns immediately. See stopAndWait if
		 * you need to sinchronize.
		 */
		void stop();

		/**
		 *  @brief  The stopAndWait method breaks the executiotn of the loop running on another thread and sinchronizes
		 * with its ending.
		 */
		void stopAndWait();
#endif

		/**
		 *  @brief  The setWindowToUpdate method adds the window with ID wID to be updated.
		 *  @param wID The ID of the Window to update.
		 */
		void setWindowToUpdate(const WindowID wID);

		/**
		 *  @brief  The process method makes this group draw the windows in windowsToUpdate or all the attached windows
		 * (if doPoll), directly or indirectly (if running concurrently).
		 */
		void process();

	  private:
		/**
		 *  @brief  The updateWindows method calls the draw method of the Windows in windowsToUpdate.
		 */
		void updateWindows();

	  public:
		/**
		 *  @brief  The newGroupID static method books a new or an old & freed ID for WindowGroups.
		 *  @return The booked WindowGroupID.
		 */
		static WindowGroupID newGroupID();

		/**
		 *  @brief  The newGroup static method allocates and creates a new WindowGroup, storing also information for
		 * later retrieval.
		 *  @return A pointer to the created WindowGroup.
		 */
		static WindowGroupPointer newGroup();

		/**
		 *  @brief  The getGroup static method returns a pointer to the WindowGroup with the given ID.
		 *  @param id The ID of the WindowGrop to retrieve.
		 *  @return A pointer to the requested WindowGroup, if it exists, or a null pointer.
		 */
		static WindowGroupPointer getGroup(const WindowGroupID id);

		/**
		 *  @brief  The getWindowGroup static method returns the ID of the group the Window identified by id belongs to.
		 *  @param id The ID of the Window.
		 *  @return The ID of the group the Window identified by id belongs to, or NoWindowGroup.
		 */
		static WindowGroupID getWindowGroup(const WindowID id);

		/**
		 *  @brief  The getAllWindowGroupsIDs static method returns the set of all the WindowGroupIDs currently in use.
		 *  @param gIDs The set of WindowGroupIDs.
		 */
		static void getAllWindowGroupIDs(std::unordered_set<WindowGroupID>& gIDs);

		/**
		 *  @brief  The getAllUngroupedWindowIDs static method returns the set of all the WindowIDs that are not
		 * attached to any group.
		 *  @param wIDs The set of WindowIDs.
		 */
		static void getAllUngroupedWindowIDs(std::unordered_set<WindowID>& wIDs);

		/**
		 *  @brief  The deleteWindowGroup static method destroys and removes the WindowGroup at id.
		 *  @param id The ID of the WindowGroup to delete.
		 */
		static void deleteWindowGroup(const WindowGroupID id);

		/**
		 *  @brief  The deleteAllWindowGroups static method destroys and removes all WindowGroup created so far.
		 */
		static void deleteAllWindowGroups();

	  private:
		/**
		 *  @brief  The WindoGroupID is the ID of this group, which is assigned at construction time and
		 *          remains constant until destruction.
		 */
		const WindowGroupID groupID;

		/**
		 *  @brief  The attachedWindows is a set of WindowIDs corresponding to the Window currently attached to this
		 * group.
		 */
		std::unordered_set<WindowID> attachedWindows;

		/**
		 *  @brief  The set of windows that must be updated at a given time.
		 */
		std::unordered_set<WindowID> windowsToUpdate;

#ifndef NO_MULTITHREADING
		/**
		 *  @brief  Determines the event processing method: true for POLL, false for WAIT.
		 */
		std::atomic<bool> doPoll;

		/**
		 *  @brief  Mutex used to guarantee correct concurrent access to this group's data.
		 */
		mutable std::mutex mutex;

		/**
		 *  @brief  Condition Variable used to wait for events.
		 */
		std::condition_variable conditonVariable;

		/**
		 *  @brief  Flag used to continue/break the concurrent execution of the loop.
		 */
		std::atomic<bool> doLoop;

		/**
		 *  @brief  Mutex used to avoid race conditions when sinchronizing for the end of concurrent execution.
		 */
		mutable std::mutex joinMutex;

		/**
		 *  @brief  Thread object to manage the concurrent execution of the loop for this WindowGroup in another thread.
		 */
		std::thread threadOfLoop;

		/**
		 *  @brief  Mutex used to guarantee correct concurrent management of static activities.
		 */
		static std::recursive_mutex globalMutex;

		/**
		 *  @brief  The concurrentLoop method is the function to be executed on another thread and that represents a
		 * loop of event processing and drawing.
		 */
		void concurrentLoop();

		/**
		 *  @brief  The waitEvents method puts the current thread to sleep if the Event queue is empty.
		 */
		void waitEvents();
#endif

		/**
		 *  @brief  The container of WindowGroups.
		 */
		static std::deque<WindowGroupPointer> windowGroups;

		/**
		 *  @brief  The container for reusing freed WindowGroupID. It is kept sorted.
		 */
		static std::deque<WindowGroupID> freedWindowGroupIDs;

		/**
		 *  @brief  The WindowGroupMap is a hash table used to map each Window to the WindowGroup it belongs to.
		 */
		using WindowGroupMap = std::unordered_map<WindowID, WindowGroupID>;

		/**
		 *  @brief  Iterator for accessing the WindowGroupMap.
		 */
		using WindowGroupMapIterator = WindowGroupMap::iterator;

		/**
		 *  @brief  The map between Windows and WindowGroups.
		 */
		static WindowGroupMap windowGroupMap;
	};

}

#endif
