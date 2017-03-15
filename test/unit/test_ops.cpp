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
#include "vectorwrapper/vectorops.hpp"
#include <gtest/gtest.h>

struct UnorderedVector {
	int64_t y;
	int64_t z;
	int64_t x;
};
namespace vwr {
	template <>
	struct VectorWrapperInfo<UnorderedVector> {
		enum { dimensions = 3 };
		typedef int64_t scalar_type;

		enum {
			offset_x = offsetof(UnorderedVector, x),
			offset_y = offsetof(UnorderedVector, y),
			offset_z = offsetof(UnorderedVector, z)
		};
	};
} //namespace vwr

typedef vwr::Vec<UnorderedVector> uvec3i;

TEST(vwr, dot) {
	using namespace vwr;

	ivec3 a(1);
	ivec3 b(1);

	EXPECT_EQ(3, dot(a, b));
	EXPECT_EQ(3, dot(a, a));

	a = ivec3{5, 2, 8};
	b = ivec3{6, 9, 3};
	EXPECT_EQ(6*5+2*9+8*3, dot(a, b));

	uvec3i c(7, 2, 9);
	EXPECT_EQ(7, c.x());
	EXPECT_EQ(2, c.y());
	EXPECT_EQ(9, c.z());
	EXPECT_EQ(7*5+2*2+9*8, dot(a, c));
	EXPECT_EQ(7*6+2*9+9*3, dot(b, c));
}

TEST(vwr, cross2D) {
	using namespace vwr;

	ivec2 a(53, 97);
	ivec2 b(71, -30);

	EXPECT_EQ(-8477, cross(a, b));

	a /= 4;
	b /= -2;
	EXPECT_EQ(1035, cross(a, b));
}

TEST(vwr, cross3D) {
	using namespace vwr;

	ivec3 a(5, 7, 13);
	ivec3 b(17, 19, 23);

	ivec3 res(-86, 106, -24);
	EXPECT_EQ(res, cross(a, b));

	a.x() *= 10;
	a.y() *= -8;
	a.z() *= 37;
	b *= -99;
	res.x() = 1032273;
	res.y() = -695673;
	res.z() = -188298;
	EXPECT_EQ(res, cross(a, b));
}
