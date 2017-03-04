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

#ifndef idE24EF7737A7F4882B2BF254F0A3EEC34
#define idE24EF7737A7F4882B2BF254F0A3EEC34

#include "vectorwrapper/sequence_bt.hpp"
#include "vectorwrapper/size_type.hpp"
#include "vectorwrapper/vectorwrapper.hpp"

#if defined VWR_OUTER_NAMESPACE
namespace VWR_OUTER_NAMESPACE {
#endif

namespace vwr {
	namespace implem {
		template <typename TO, typename FROM, size_type... I>
		Vec<TO> vector_cast (const Vec<FROM>& parVec, Vec<TO>*, bt::number_seq<size_type, I...>) {
			static_assert(
				static_cast<int>(Vec<TO>::dimensions) == static_cast<int>(Vec<FROM>::dimensions),
				"Mismatching dimensions"
			);
			static_assert(sizeof...(I) == Vec<TO>::dimensions, "Mismatching index count");
			typedef typename Vec<TO>::scalar_type CastType;

			return Vec<TO>(static_cast<CastType>(parVec[I])...);
		}
	} //namespace implem

	template <typename TOVec, typename FROM>
	TOVec vector_cast (const Vec<FROM>& parVec) {
		TOVec* const to = nullptr;
		return implem::vector_cast(parVec, to, bt::number_range<size_type, 0, Vec<FROM>::dimensions>());
	}
} //namespace vwr

#if defined VWR_OUTER_NAMESPACE
} //namespace VWR_OUTER_NAMESPACE
#endif

#endif
