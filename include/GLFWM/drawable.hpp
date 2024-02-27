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

#ifndef GLFWM_DRAWABLE_HPP
#define GLFWM_DRAWABLE_HPP

#include <GLFWM/enums.hpp>

namespace glfwm {

	/// The Drawable class represents objects that can be rendered in a window. Inherit this class and bind its objects
	/// to a window to be displayed.
	class Drawable {
	  public:
		virtual ~Drawable() = default;

		/**
		 *    @brief  The draw method is responsible of performing the rendering of an object of this class. It must be
		 * implemented in derived classes.
		 *    @param id The ID of the window calling this method at a given time.
		 *    @note   Any Drawable object can be bound to any number of different windows. The id parameter can be used
		 * to locate resources associated to it, e.g. OpenGL buffer objects.
		 */
		virtual void draw(const WindowID id) = 0;
	};

	/**
	 *    @brief  A smart pointer to a Drawable. To bind a Drawable to a Window, a smart pointer must be used.
	 */
	using DrawablePointer = std::shared_ptr<Drawable>;
}

#endif
