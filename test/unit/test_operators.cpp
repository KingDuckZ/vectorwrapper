#include "sample_vectors.hpp"
#include <gtest/gtest.h>

TEST(vwr, operators) {
	using namespace vwr;

	{
		ivec3 a(5, 6, 7);
		ivec3 b(6, 7, 8);

		EXPECT_LT(a, b);
		EXPECT_LE(a, b);
		EXPECT_NE(a, b);
		EXPECT_FALSE(a == b);
		EXPECT_FALSE(a > b);
		EXPECT_FALSE(a >= b);
		EXPECT_GT(b, a);
		EXPECT_GE(b, a);
	}
	{
		ivec3 a(6, 6, 7);
		ivec3 b(6, 7, 8);

		EXPECT_FALSE(a < b);
		EXPECT_LE(a, b);
		EXPECT_NE(a, b);
		EXPECT_FALSE(a == b);
		EXPECT_FALSE(a > b);
		EXPECT_FALSE(a >= b);
		EXPECT_GE(b, a);
	}
	{
		ivec3 a(0xAABB, 0xAABB, 0xAABB);
		ivec3 b(0xAABB, 0xAABB, 0xAABB);

		EXPECT_FALSE(a < b);
		EXPECT_LE(a, b);
		EXPECT_FALSE(a != b);
		EXPECT_EQ(a, b);
		EXPECT_FALSE(a > b);
		EXPECT_GE(a, b);
		EXPECT_GE(b, a);
	}
}
