#include "sample_vectors.hpp"
#include <gtest/gtest.h>

TEST(vwr, conversion) {
	using namespace vwr;

	{
		svec1 s(10.0f);
		EXPECT_EQ(s.x(), 10.0f);

		auto s2 = s.x1();
		static_assert(std::is_same<decltype(s2), svec2>::value, "Expecting svec2");
		EXPECT_EQ(s2.x(), s.x());
		EXPECT_EQ(s2.y(), 1.0f);
	}
}
