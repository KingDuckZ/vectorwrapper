/*
 * Copyright 2015, 2016 Michele "King_DuckZ" Santullo
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

#ifndef id2996A9D174804F7B88936D6C363D235B
#define id2996A9D174804F7B88936D6C363D235B

#include "vectorwrapper/vectorwrapper.hpp"
#include <type_traits>
#include <emmintrin.h>

namespace vwr {
	namespace simd {
		template <typename V, std::size_t=::vwr::Vec<V>::dimensions, typename=typename ::vwr::Vec<V>::scalar_type>
		class Vec;

		template <typename V>
		class Vec<V, 3, float> : public ::vwr::Vec<V, 3> {
			typedef ::vwr::Vec<V, 3> base_class;
		public:
			static_assert(alignof(V) % 16 == 0, "Wrapped type must be aligned to 16");
			static_assert(base_class::is_interleaved_mem == 0, "Expected tightly packed vector_type");

			using typename base_class::vector_type;
			typedef float scalar_type;

			Vec ( void ) = default;
			Vec ( const Vec& ) = default;
			explicit Vec ( const vector_type& parIn ) : base_class(parIn) { }
			explicit Vec ( const scalar_type parX ) : base_class(parX) { }
			explicit Vec ( const base_class& parIn ) : base_class(parIn) { }

			template <typename V2> Vec& operator+= ( const Vec<V2, 3, scalar_type>& parOther );
			template <typename V2> Vec& operator-= ( const Vec<V2, 3, scalar_type>& parOther );
			template <typename V2> Vec& operator*= ( const Vec<V2, 3, scalar_type>& parOther );
			template <typename V2> Vec& operator/= ( const Vec<V2, 3, scalar_type>& parOther );
		};
	} //namespace simd
} //namespace vwr

#include "vectorwrapper/vectorwrapper_simd.inl"

#endif
