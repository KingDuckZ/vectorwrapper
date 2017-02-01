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

#include "vectorwrapper/vectorwrapper.hpp"
#include "vectorwrapper/size_type.hpp"
#include <type_traits>
#include <cstddef>

namespace vwr {
	template <typename V1, typename V2, size_type S>
	typename std::common_type<typename Vec<V1>::scalar_type, typename Vec<V2>::scalar_type>::type dot ( const Vec<V1, S>& parLeft, const Vec<V2, S>& parRight );

	template <typename V1, typename V2>
	typename std::common_type<typename Vec<V1>::scalar_type, typename Vec<V2>::scalar_type>::type cross ( const Vec<V1, 2>& parLeft, const Vec<V2, 2>& parRight );
	template <typename V1, typename V2>
	Vec<typename std::common_type<V1, V2>::type> cross ( const Vec<V1, 3>& parLeft, const Vec<V2, 3>& parRight );

	template <typename V1, typename V2, size_type S>
	inline typename std::common_type<typename Vec<V1>::scalar_type, typename Vec<V2>::scalar_type>::type dot (const Vec<V1, S>& parLeft, const Vec<V2, S>& parRight) {
		auto retval = parLeft.x() * parRight.x();
		for (size_type z = 1; z < S; ++z) {
			retval += parLeft[z] * parRight[z];
		}
		return retval;
	}

	template <typename V1, typename V2>
	inline typename std::common_type<typename Vec<V1>::scalar_type, typename Vec<V2>::scalar_type>::type cross (const Vec<V1, 2>& parLeft, const Vec<V2, 2>& parRight) {
		return parLeft.x() * parRight.y() - parLeft.y() * parRight.x();
	}
	template <typename V1, typename V2>
	inline Vec<typename std::common_type<V1, V2>::type> cross (const Vec<V1, 3>& parLeft, const Vec<V2, 3>& parRight) {
		return Vec<typename std::common_type<V1, V2>::type>(
			parLeft.y() * parRight.z() - parLeft.z() * parRight.y(),
			parLeft.z() * parRight.x() - parLeft.x() * parRight.z(),
			parLeft.x() * parRight.y() - parLeft.y() * parRight.x()
		);
	}
} //namespace vwr

#endif
