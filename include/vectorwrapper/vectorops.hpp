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
