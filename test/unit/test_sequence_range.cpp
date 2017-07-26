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

#include "sample_vectors.hpp"
#include "vectorwrapper/sequence_range.hpp"
#include <gtest/gtest.h>
#include <functional>
#include <array>
#include <vector>

TEST(vwr, vector_iterator) {
	using namespace vwr;

	ivec3 from(100, 200, 300);
	ivec3 to(102, 203, 304);

	std::array<ivec3, 24> expected{
		ivec3(100, 200, 300),
		ivec3(101, 200, 300),
		ivec3(100, 201, 300),
		ivec3(101, 201, 300),
		ivec3(100, 202, 300),
		ivec3(101, 202, 300),
		ivec3(100, 200, 301),
		ivec3(101, 200, 301),
		ivec3(100, 201, 301),
		ivec3(101, 201, 301),
		ivec3(100, 202, 301),
		ivec3(101, 202, 301),
		ivec3(100, 200, 302),
		ivec3(101, 200, 302),
		ivec3(100, 201, 302),
		ivec3(101, 201, 302),
		ivec3(100, 202, 302),
		ivec3(101, 202, 302),
		ivec3(100, 200, 303),
		ivec3(101, 200, 303),
		ivec3(100, 201, 303),
		ivec3(101, 201, 303),
		ivec3(100, 202, 303),
		ivec3(101, 202, 303)
	};

	{
		auto seq = increasing_sequence_range<ivec3>(from, to);
		auto it = seq.begin();
		auto it_end = seq.end();

		std::vector<ivec3> results;
		for (; it != it_end; ++it) {
			results.push_back(*it);
		}

		EXPECT_EQ(expected.size(), results.size());
		for (std::size_t z = 0; z < expected.size(); ++z) {
			EXPECT_EQ(expected[z], results[z]);
		}
	}

	{
		using dec_sequence_range = sequence_range<ivec3, op::dec<int64_t>, std::greater<int64_t>, 0, 1, 2>;
		std::vector<ivec3> results;
		for (auto& itm : dec_sequence_range(to, from)) {
			results.push_back(itm);
		}

		EXPECT_EQ(expected.size(), results.size());
		for (std::size_t z = 0; z < expected.size(); ++z) {
			EXPECT_EQ(expected[z] + 1, results[results.size() - 1 - z]);
		}
	}
}
