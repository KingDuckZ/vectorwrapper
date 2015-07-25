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

TEST(vwr, ops) {
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
