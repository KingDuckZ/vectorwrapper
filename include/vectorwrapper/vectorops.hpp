/*
 * Copyright 2015-2016 Michele "King_DuckZ" Santullo
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

#ifndef id8949C80C36BA42CABC49EA4C1DB54BC7
#define id8949C80C36BA42CABC49EA4C1DB54BC7

#include "vectorwrapper.hpp"
#include <type_traits>
#include <cstddef>

namespace vwr {
	template <typename V1, typename V2, std::size_t S>
	typename std::common_type<typename Vec<V1>::scalar_type, typename Vec<V2>::scalar_type>::type dot ( const Vec<V1, S>& parLeft, const Vec<V2, S>& parRight );

	template <typename V1, typename V2, std::size_t S>
	inline typename std::common_type<typename Vec<V1>::scalar_type, typename Vec<V2>::scalar_type>::type dot (const Vec<V1, S>& parLeft, const Vec<V2, S>& parRight) {
		auto retval = parLeft.x() * parRight.x();
		for (std::size_t z = 1; z < S; ++z) {
			retval += parLeft[z] * parRight[z];
		}
		return retval;
	}
} //namespace vwr

#endif
