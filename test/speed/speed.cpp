#include "vectorwrapper/vectorops.hpp"
#include "vectorwrapper/vectorwrapper_simd.hpp"
#include <gtest/gtest.h>
#include <memory>

namespace vwr {
	typedef std::aligned_storage<sizeof(float) * 3, 16>::type float3_storage;

	template <>
	struct VectorWrapperInfo<float3_storage> {
		enum { dimensions = 3 };
		typedef float scalar_type;

		enum {
			offset_x = 0,
			offset_y = offset_x + sizeof(scalar_type),
			offset_z = offset_y + sizeof(scalar_type)
		};
	};
} //namespace vwr

TEST(vwr_speed, speed) {
	typedef vwr::simd::Vec<vwr::float3_storage> simd_vec3;
	static_assert(sizeof(simd_vec3) >= sizeof(float) * 3, "SIMD vector too small");

	simd_vec3 v1(1.0f);
	simd_vec3 v2(0.5f);

	v1 += v2;

	EXPECT_FLOAT_EQ(1.5f, v1.x());
	EXPECT_FLOAT_EQ(1.5f, v1.y());
	EXPECT_FLOAT_EQ(1.5f, v1.z());
}
