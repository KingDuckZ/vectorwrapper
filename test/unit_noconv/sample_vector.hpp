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

#ifndef idD52B2591EB8240A980C5B3823D025EC1
#define idD52B2591EB8240A980C5B3823D025EC1

#include "vectorwrapper/vectorwrapper.hpp"
#include <array>

#define SPECIALIZE_ARRAY_VECTOR(TYPE, DIM) \
    template <> \
    struct VectorWrapperInfo<std::array<TYPE, DIM>> { \
        enum { dimensions = DIM }; \
        typedef TYPE scalar_type; \
        typedef std::array<scalar_type, dimensions> vector_type; \
        static scalar_type& get_at (size_t parIndex, vector_type& parVector) { \
            return parVector[parIndex]; \
        } \
    }

namespace vwr_outer_ns {
namespace vwr {
	SPECIALIZE_ARRAY_VECTOR(float, 2);
	SPECIALIZE_ARRAY_VECTOR(float, 3);
	SPECIALIZE_ARRAY_VECTOR(short int, 2);
	SPECIALIZE_ARRAY_VECTOR(short int, 3);

	typedef Vec<std::array<float, 2>> float2;
	typedef Vec<std::array<float, 3>> float3;
	typedef Vec<std::array<short int, 2>> short2;
	typedef Vec<std::array<short int, 3>> short3;
} //namespace vwr
} //namespace vwr_outer_ns

#undef SPECIALIZE_ARRAY_VECTOR

#endif
