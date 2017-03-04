#include "sample_vector.hpp"
#include "vectorwrapper/vector_cast.hpp"
#include <gtest/gtest.h>

TEST(vwr, vector_cast) {
	using namespace vwr_outer_ns::vwr;

	{
		float2 v1(1.0f, 2.0f);
		short2 v2(vector_cast<short2>(v1));

		EXPECT_EQ(1, v2.x());
		EXPECT_EQ(2, v2.y());
	}

	{
		short3 v1(3, 4, 5);
		auto v2 = vector_cast<float3>(v1) / float3(2.0f);

		EXPECT_FLOAT_EQ(1.5f, v2.x());
		EXPECT_FLOAT_EQ(2.0f, v2.y());
		EXPECT_FLOAT_EQ(2.5f, v2.z());
	}

	{
		short3 v1(0xAB, 0xCD, 0xEF);
		short3 v2(0x12, 0x34, 0x56);
		float3 v3(vector_cast<float3>(v1 + v2));

		EXPECT_FLOAT_EQ(static_cast<float>(0xAB + 0x12), v3.x());
		EXPECT_FLOAT_EQ(static_cast<float>(0xCD + 0x34), v3.y());
		EXPECT_FLOAT_EQ(static_cast<float>(0xEF + 0x56), v3.z());

		short3 v4 = vector_cast<short3>(v3);
		EXPECT_EQ(0xAB + 0x12, v4.x());
		EXPECT_EQ(0xCD + 0x34, v4.y());
		EXPECT_EQ(0xEF + 0x56, v4.z());
	}
}

TEST(vwr, promotion_static_cast) {
	using namespace vwr_outer_ns::vwr;

	{
		short2 v1(100, 200);
		auto v2 = v1 / short2(2);
		EXPECT_EQ(50, v2.x());
		EXPECT_EQ(100, v2.y());
	}

	{
		short2 v1(100, 200);
		auto v2 = v1 * short2(2);
		EXPECT_EQ(200, v2.x());
		EXPECT_EQ(400, v2.y());
	}

	{
		short2 v1(100, 200);
		auto v2 = v1 + short2(2);
		EXPECT_EQ(102, v2.x());
		EXPECT_EQ(202, v2.y());
	}

	{
		short2 v1(100, 200);
		auto v2 = v1 - short2(2);
		EXPECT_EQ(98, v2.x());
		EXPECT_EQ(198, v2.y());
	}
}
