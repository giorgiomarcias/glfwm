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

#ifndef GLFWM_UTILITY_HPP
#define GLFWM_UTILITY_HPP

#include <GLFWM/common.hpp>

namespace glfwm {

	/**
	 *  @brief  The ObjectRank struct stores an object and a rank which determines its position in lists.
	 */
	template <typename ObjectType, typename RankType = int>
	struct ObjectRank {
		ObjectType object;
		RankType rank;

		/**
		 *  @brief  The overloaded operator < is used for sorting.
		 *  @param r The ObjectRank to compare this object with.
		 *  @return true if this object comes before r, false otherwise.
		 */
		bool operator<(const ObjectRank& r) const { return rank < r.rank; }
	};

}

#endif
