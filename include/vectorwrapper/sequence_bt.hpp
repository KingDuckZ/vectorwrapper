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

#pragma once

#include <cstddef>

#if defined VWR_OUTER_NAMESPACE
namespace VWR_OUTER_NAMESPACE {
#endif

namespace vwr {
	namespace bt {
		template <typename T, T... I>
		struct number_seq {
		};

		template <std::size_t... I>
		using index_seq = number_seq<std::size_t, I...>;

		namespace implem {
			template <typename T, T MIN, T MAX, T... I>
			struct range_builder;

			template <typename T, T MIN, T... I>
			struct range_builder<T, MIN, MIN, I...> {
				typedef number_seq<T, I...> type;
			};

			template <typename T, T MIN, T N, T... I>
			struct range_builder : public range_builder<T, MIN, N - 1, N - 1, I...> {
			};
		} //namespace implem

		template <typename T, T MIN, T MAX>
		using number_range = typename implem::range_builder<T, MIN, MAX>::type;

		template <std::size_t MIN, std::size_t MAX>
		using index_range = number_range<std::size_t, MIN, MAX>;
	} //namespace bt
} //namespace vwr

#if defined VWR_OUTER_NAMESPACE
} //namespace VWR_OUTER_NAMESPACE
#endif
