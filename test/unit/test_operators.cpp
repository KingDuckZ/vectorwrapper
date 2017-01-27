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

TEST(vwr, bin_operators_scalar) {
	using namespace vwr;

	{
		ivec3 a(50, 100, 150);
		ivec3 res(50 + 10, 100 + 10, 150 + 10);
		EXPECT_EQ(res, a + 10);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(50 - 25, 100 - 25, 150 - 25);
		EXPECT_EQ(res, a - 25);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(50 * 10, 100 * 10, 150 * 10);
		EXPECT_EQ(res, a * 10);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(50 / 2, 100 / 2, 150 / 2);
		EXPECT_EQ(res, a / 2);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(50 % 7, 100 % 7, 150 % 7);
		EXPECT_EQ(res, a % 7);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(25 + 50, 25 + 100, 25 + 150);
		EXPECT_EQ(res, 25 + a);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(25 - 50, 25 - 100, 25 - 150);
		EXPECT_EQ(res, 25 - a);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(4 * 50, 4 * 100, 4 * 150);
		EXPECT_EQ(res, 4 * a);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(1000 / 50, 1000 / 100, 1000 / 150);
		EXPECT_EQ(res, 1000 / a);
	}
	{
		ivec3 a(50, 100, 150);
		ivec3 res(1000 % 50, 1000 % 100, 1000 % 150);
		EXPECT_EQ(res, 1000 % a);
	}
}

TEST(vwr, bin_assign_op) {
	using namespace vwr;

	{
		ivec3 a(2, 4, 8);
		ivec3 res(2 + 20, 4 + 20, 8 + 20);
		a += ivec3(20);
		EXPECT_EQ(res, a);
	}
	{
		ivec3 a(2, 4, 8);
		ivec3 res(2 - 20, 4 - 20, 8 - 20);
		a -= ivec3(20);
		EXPECT_EQ(res, a);
	}
	{
		ivec3 a(2, 4, 8);
		ivec3 res(2 * 20, 4 * 20, 8 * 20);
		a *= ivec3(20);
		EXPECT_EQ(res, a);
	}
	{
		ivec3 a(2, 4, 8);
		ivec3 res(2 / 2, 4 / 2, 8 / 2);
		a /= ivec3(2);
		EXPECT_EQ(res, a);
	}
}

TEST(vwr, bin_assign_op_scalar) {
	using namespace vwr;

	{
		ivec3 a(2, 4, 8);
		ivec3 res(2 + 20, 4 + 20, 8 + 20);
		a += 20;
		EXPECT_EQ(res, a);
	}
	{
		ivec3 a(2, 4, 8);
		ivec3 res(2 - 20, 4 - 20, 8 - 20);
		a -= 20;
		EXPECT_EQ(res, a);
	}
	{
		ivec3 a(2, 4, 8);
		ivec3 res(2 * 20, 4 * 20, 8 * 20);
		a *= 20;
		EXPECT_EQ(res, a);
	}
	{
		ivec3 a(2, 4, 8);
		ivec3 res(2 / 2, 4 / 2, 8 / 2);
		a /= 2;
		EXPECT_EQ(res, a);
	}
}
