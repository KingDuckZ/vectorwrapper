#include "sample_vectors.hpp"
#include <gtest/gtest.h>

namespace {
	void test_svec3 (const vwr::svec3& parVec, float parX, float parY, float parZ) {
		EXPECT_EQ(parVec.x(), parX);
		EXPECT_EQ(parVec.y(), parY);
		EXPECT_EQ(parVec.z(), parZ);
	}

	void test_tvec3 (const vwr::tvec3& parVec, float parX, float parY, float parZ) {
		EXPECT_EQ(parVec.x(), parX);
		EXPECT_EQ(parVec.y(), parY);
		EXPECT_EQ(parVec.z(), parZ);
	}
} //unnamed namespace

TEST(vwr, conversion) {
	using namespace vwr;

	{
		svec1 s(10.0f);
		EXPECT_EQ(s.x(), 10.0f);

		auto s2 = s.x1();
		static_assert(std::is_same<decltype(s2), svec2>::value, "Expecting svec2");
		EXPECT_EQ(s2.x(), s.x());
		EXPECT_EQ(s2.y(), 1.0f);

		auto s3 = s2.xyn(2.0f);
		EXPECT_EQ(s3.x(), s2.x());
		EXPECT_EQ(s3.y(), s2.y());
		EXPECT_EQ(s3.z(), 2.0f);

		const auto s3_copy(s3);
		EXPECT_EQ(s3_copy, s3);
	}

	{
		svec3 s3(1.0f);
		mvec3 m3(s3);
		EXPECT_EQ(m3.x(), 1.0f);
		EXPECT_EQ(m3.y(), 1.0f);
		EXPECT_EQ(m3.z(), 1.0f);
		EXPECT_EQ(m3.data().x, 1.0f);
		EXPECT_EQ(m3.data().y, 1.0f);
		EXPECT_EQ(m3.data().z, 1.0f);
	}

	{
		pvec3 p3(1.0f, 2.0f, 3.0f);
		test_svec3(p3.cast<svec3>(), p3.x(), p3.y(), p3.z());
		test_tvec3(p3.cast<tvec3>(), p3.x(), p3.y(), p3.z());
	}
}

TEST(vwr, cast) {
	using namespace vwr;

	svec2 s(10.0f);
	ivec2 i = static_cast<ivec2>(s);
	EXPECT_EQ(i.x(), 10);
	EXPECT_EQ(i.y(), 10);
}
