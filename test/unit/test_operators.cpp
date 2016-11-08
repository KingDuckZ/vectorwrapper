#include "sample_vectors.hpp"
#include <gtest/gtest.h>

TEST(vwr, cmp_operators) {
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

TEST(vwr, bin_operators) {
	using namespace vwr;

	{
		ivec3 a(0xFF, 0xAB, 0x10CE);
		ivec3 b(0x45, 0xEE, 0x8);
		ivec3 res(0xFF + 0x45, 0xAB + 0xEE, 0x10CE + 0x8);

		EXPECT_EQ(res, a + b);
	}
	{
		ivec3 a(0xFF, 0xAB, 0x10CE);
		ivec3 b(0xC0, 0x0, 0xA);
		ivec3 res(0xFF - 0xC0, 0xAB - 0x0, 0x10CE - 0xA);

		EXPECT_EQ(res, a - b);
	}
	{
		ivec3 a(0xFF, 0xAB, 0x10CE);
		ivec3 b(0x3, 0x2, 0x1);
		ivec3 res(0xFF * 0x3, 0xAB * 0x2, 0x10CE * 0x1);

		EXPECT_EQ(res, a * b);
	}
	{
		ivec3 a(0xFF, 0xAB, 0x10CE);
		ivec3 b(0x3, 0x2, 0x1);
		ivec3 res(0xFF / 0x3, 0xAB / 0x2, 0x10CE / 0x1);

		EXPECT_EQ(res, a / b);
	}
	{
		ivec3 a(0xE9, 0x104A, 0x28FF);
		ivec3 b(0x15, 0x20, 0x1000);
		ivec3 res(0xE9 % 0x15, 0x104A % 0x20, 0x28FF % 0x1000);

		EXPECT_EQ(res, a % b);
	}
}
