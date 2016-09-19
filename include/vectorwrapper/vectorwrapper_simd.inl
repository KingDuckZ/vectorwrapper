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

namespace vwr {
	namespace simd {
		template <typename V>
		template <typename V2>
		auto Vec<V, 3, float>::operator+= (const Vec<V2, 3, scalar_type>& parOther) -> Vec& {
			__m128 pack_this;
			__m128 pack_other;

			pack_this = _mm_load_ps(&this->x());
			pack_other = _mm_load_ps(&parOther.x());
			pack_this = _mm_add_ps(pack_this, pack_other);
			_mm_store_ps(&this->x(), pack_this);

			return *this;
		}

		template <typename V>
		template <typename V2>
		auto Vec<V, 3, float>::operator-= (const Vec<V2, 3, scalar_type>& parOther) -> Vec& {
			__m128 pack_this;
			__m128 pack_other;

			pack_this = _mm_load_ps(&this->x());
			pack_other = _mm_load_ps(&parOther.x());
			pack_this = _mm_sub_ps(pack_this, pack_other);
			_mm_store_ps(&this->x(), pack_this);

			return *this;
		}
	} //namespace simd
} //namespace vwr
