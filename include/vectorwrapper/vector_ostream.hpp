/*
 * Copyright 2015-2017 Michele "King_DuckZ" Santullo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "vectorwrapper.hpp"
#include "size_type.hpp"
#include <iostream>

namespace vwr {
	template <typename T, size_type S>
	inline
	std::ostream& operator<< (std::ostream& parStream, const Vec<T, S>& parVec) {
		static_assert(S > 0, "Invalid size");

		parStream << '<';
		for (size_type z = 0; z < S - 1; ++z) {
			parStream << parVec[z] << ',';
		}
		parStream << parVec[S - 1] << '>';
		return parStream;
	}
} //namespace vwr
