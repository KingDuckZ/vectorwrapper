#include "sample_vectors.hpp"
#include <gtest/gtest.h>

TEST(vwr, conversion) {
	using namespace vwr;

	{
		svec1 s(10.0f);
		EXPECT_EQ(s.x(), 10.0f);
	}
}
