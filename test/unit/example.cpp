#include "sample_vectors.hpp"
#include <gtest/gtest.h>

namespace {
	void test_svec2 (const vwr::svec2& parVec, float parX, float parY) {
		EXPECT_EQ(parVec.x(), parX);
		EXPECT_EQ(parVec.y(), parY);
	}

	void simplevec_double (vwr::SimpleVector3& parVec, float parX, float parY, float parZ) {
		EXPECT_EQ(parVec.x, parX);
		EXPECT_EQ(parVec.y, parY);
		EXPECT_EQ(parVec.z, parZ);

		parVec.x *= 2.0f;
		parVec.y *= 2.0f;
		parVec.z *= 2.0f;
	}
} //unnamed namespace

TEST(vwr, example) {
	using namespace vwr;

	//Using the bare vectors
	{
		//Imagine you get this vector from a function's return value.
		auto vec = TrailingDataVector3{2.0f, 5.5f, 3.8f, 1, 2};

		//Now you want to pass it on to a function from a different library.
		//To be safe, create a temporary of the correct type.
		//Was it lower case x, upper case, with or without paretheses?
		auto vec_b = SimpleVector3{vec.X, vec.Y, vec.Z};
		simplevec_double(vec_b, vec_b.x, vec_b.y, vec_b.z);

		//The above call changed the input vector, so re-assign it (vec_b is
		//just a converted copy, remember?)
		vec.X = vec_b.x;
		vec.Y = vec_b.y;
		vec.Z = vec_b.z;

		//Can't compare with vec == vec_b, so unroll it manually
		EXPECT_EQ(vec.X, vec_b.x);
		EXPECT_EQ(vec.Y, vec_b.y);
		EXPECT_EQ(vec.Z, vec_b.z);
	}

	//Using vectorwrapper
	{
		auto vec_in = TrailingDataVector3{2.0f, 5.5f, 3.8f, 1, 2};

		//In reality, I just wrote TrailingDataVector3's definition myself, but
		//I'm already spending time to fix the build as I call vec.x instead of
		//vec.X. Do you think it's a remote case? Think twice.
		EXPECT_EQ(vec_in.X, 2.0f);
		EXPECT_EQ(vec_in.Y, 5.5f);
		EXPECT_EQ(vec_in.Z, 3.8f);
		EXPECT_EQ(vec_in.a, 1);
		EXPECT_EQ(vec_in.b, 2);

		tvec3 vec(vec_in);
		EXPECT_EQ(vec.x(), 2.0f);
		EXPECT_EQ(vec.y(), 5.5f);
		EXPECT_EQ(vec.z(), 3.8f);
		//If you initialize vec from an object of the wrapped type, extra data
		//also gets initialized correctly. But I think in most situations you
		//won't need this.
		EXPECT_EQ(vec.data().a, 1);
		EXPECT_EQ(vec.data().b, 2);

		//Suppose TrailingDataVector3 is a third-party struct and I'm not
		//guaranteed it will never change. Sure I can reinterpret_cast it, as
		//much as I could reinterpret_cast a short int to a TrailingDataVector3.
		//The following line won't compile if casting is not deemed appropriate.
		simplevec_double(vec.cast<vwr::svec3>().data(), vec.x(), vec.y(), vec.z());

		//Well that's it.

		//If what you wanted is indeed a copy of type SimpleVector3, here we go:
		svec3 vec_b(vec);
		EXPECT_EQ(vec_b, vec);
	}

	{
		//You can also cast a svec3 to svec2, if you need to.
		const svec3 vec(9.5f, 1.2f, 0.9f);
		test_svec2(vec.cast<svec2>(), vec.x(), vec.y());
	}
}
