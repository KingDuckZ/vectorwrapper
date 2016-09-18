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

#ifndef id4FAEF395B9ED47CB9D6B50B54C9A289A
#define id4FAEF395B9ED47CB9D6B50B54C9A289A

#include <cstddef>

namespace vwr {
	namespace bt {
		template <std::size_t... I>
		struct index_seq {
		};

		namespace implem {
			template <std::size_t MIN, std::size_t MAX, std::size_t... I>
			struct range_builder;

			template <std::size_t MIN, std::size_t... I>
			struct range_builder<MIN, MIN, I...> {
				typedef index_seq<I...> type;
			};

			template <std::size_t MIN, std::size_t N, std::size_t... I>
			struct range_builder : public range_builder<MIN, N - 1, N - 1, I...> {
			};
		} //namespace implem

		template <std::size_t MIN, std::size_t MAX>
		using index_range = typename implem::range_builder<MIN, MAX>::type;
	} //namespace bt
} //namespace vwr

#endif
