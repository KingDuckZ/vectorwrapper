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

#include "vectorwrapper/vectorwrapper.hpp"
#include <gtest/gtest.h>
#include <array>
#include <cstdint>

namespace vwr {
	template <>
	struct VectorWrapperInfo<std::array<uint16_t, 2>> {
        enum {
            dimensions = 2
        };

        typedef uint16_t scalar_type;

        typedef std::array<scalar_type, dimensions> vector_type;

        typedef std::array<scalar_type, dimensions + 1> higher_vector_type;

        static scalar_type& get_at (size_t parIndex, vector_type& parVector) {
            return parVector[parIndex];
        }
	};
	template <>
	struct VectorWrapperInfo<std::array<uint16_t, 3>> {
        enum {
            dimensions = 3
        };

        typedef uint16_t scalar_type;

        typedef std::array<scalar_type, dimensions> vector_type;

        typedef std::array<scalar_type, dimensions - 1> lower_vector_type;

        static scalar_type& get_at (size_t parIndex, vector_type& parVector) {
            return parVector[parIndex];
        }
	};
} //namespace vwr

using vec2 = vwr::Vec<std::array<uint16_t, 2>>;
using vec3 = vwr::Vec<std::array<uint16_t, 3>>;

TEST(vwr, get_at) {
	const uint16_t v2 = 0x0c0cU;
	vec2 myvec2(v2);
	EXPECT_EQ(v2, myvec2.x());
	EXPECT_EQ(v2, myvec2.y());

	const uint16_t v3 = 0xababU;
	vec3 myvec3(myvec2.x(), myvec2.y(), v3);
	EXPECT_EQ(v2, myvec3.x());
	EXPECT_EQ(v2, myvec3.y());
	EXPECT_EQ(v3, myvec3.z());
}
