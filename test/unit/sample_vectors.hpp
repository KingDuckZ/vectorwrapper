#ifndef id050D92FF58A04FCBB4ACD5FB1C73071F
#define id050D92FF58A04FCBB4ACD5FB1C73071F

#include "vectorwrapper/vectorwrapper.hpp"
#include <cstdint>
#include <cstddef>

namespace vwr {
	struct SimpleVector2 {
		float x;
		float y;
	};
	struct SimpleVector3 {
		float x;
		float y;
		float z;
	};

	struct IntVector1 {
		int64_t x;
	};
	struct IntVector2 {
		int64_t x;
		int64_t y;
	};
	struct IntVector3 {
		int64_t x;
		int64_t y;
		int64_t z;
	};

	struct MixedDataVector3 {
		float s;
		float x;
		float x1;
		float y;
		float y1;
		float z;
		float z1;
	};

	struct PaddedVector3 {
		float a;
		float b;
		float x;
		float y;
		float z;
		int c, d;
	};

	struct TrailingDataVector3 {
		float X;
		float Y;
		float Z;
		int a, b;
	};

	template <>
	struct VectorWrapperInfo<float> {
		enum { dimensions = 1 };
		typedef float vector_type;
		typedef float scalar_type;
		typedef SimpleVector2 higher_vector_type;

		static scalar_type& get_at (size_type, vector_type& parVector) {
			return parVector;
		}
	};
	template <>
	struct VectorWrapperInfo<SimpleVector2> {
		enum { dimensions = 2 };

		typedef float scalar_type;
		typedef SimpleVector3 higher_vector_type;
		typedef float lower_vector_type;

		enum {
			offset_x = offsetof(SimpleVector2, x),
			offset_y = offsetof(SimpleVector2, y)
		};
	};
	template <>
	struct VectorWrapperInfo<SimpleVector3> {
		enum { dimensions = 3 };

		typedef float scalar_type;
		typedef SimpleVector2 lower_vector_type;

		enum {
			offset_x = offsetof(SimpleVector3, x),
			offset_y = offsetof(SimpleVector3, y),
			offset_z = offsetof(SimpleVector3, z)
		};
	};

	template <>
	struct VectorWrapperInfo<IntVector1> {
		enum { dimensions = 1 };
		typedef decltype(IntVector1::x) scalar_type;
		typedef IntVector2 higher_vector_type;

		enum {
			offset_x = offsetof(IntVector1, x)
		};
	};
	template <>
	struct VectorWrapperInfo<IntVector2> {
		enum { dimensions = 2 };
		typedef VectorWrapperInfo<IntVector1>::scalar_type scalar_type;
		typedef IntVector1 lower_vector_type;
		typedef IntVector3 higher_vector_type;

		enum {
			offset_x = offsetof(IntVector2, x),
			offset_y = offsetof(IntVector2, y)
		};
	};
	template <>
	struct VectorWrapperInfo<IntVector3> {
		enum { dimensions = 3 };
		typedef VectorWrapperInfo<IntVector1>::scalar_type scalar_type;
		typedef IntVector2 lower_vector_type;

		enum {
			offset_x = offsetof(IntVector3, x),
			offset_y = offsetof(IntVector3, y),
			offset_z = offsetof(IntVector3, z)
		};
	};

	template <>
	struct VectorWrapperInfo<MixedDataVector3> {
		enum { dimensions = 3 };
		typedef float scalar_type;

		enum {
			offset_x = offsetof(MixedDataVector3, x),
			offset_y = offsetof(MixedDataVector3, y),
			offset_z = offsetof(MixedDataVector3, z)
		};
	};

	template <>
	struct VectorWrapperInfo<PaddedVector3> {
		enum { dimensions = 3 };
		typedef float scalar_type;

		enum {
			offset_x = offsetof(PaddedVector3, x),
			offset_y = offsetof(PaddedVector3, y),
			offset_z = offsetof(PaddedVector3, z)
		};
	};

	template <>
	struct VectorWrapperInfo<TrailingDataVector3> {
		enum {
			dimensions = 3,
			cast_ignore_trailing_properties = 1
		};
		typedef float scalar_type;

		enum {
			offset_x = offsetof(TrailingDataVector3, X),
			offset_y = offsetof(TrailingDataVector3, Y),
			offset_z = offsetof(TrailingDataVector3, Z)
		};
	};

	typedef Vec<float> svec1;
	typedef Vec<SimpleVector2> svec2;
	typedef Vec<SimpleVector3> svec3;
	typedef Vec<IntVector1> ivec1;
	typedef Vec<IntVector2> ivec2;
	typedef Vec<IntVector3> ivec3;
	typedef Vec<MixedDataVector3> mvec3;
	typedef Vec<PaddedVector3> pvec3;
	typedef Vec<TrailingDataVector3> tvec3;

	static_assert(sizeof(svec1) == sizeof(float), "Wrong size");
	static_assert(sizeof(svec2) == sizeof(SimpleVector2), "Wrong size");
	static_assert(sizeof(svec3) == sizeof(SimpleVector3), "Wrong size");
	static_assert(sizeof(ivec1) == sizeof(IntVector1), "Wrong size");
	static_assert(sizeof(ivec2) == sizeof(IntVector2), "Wrong size");
	static_assert(sizeof(ivec3) == sizeof(IntVector3), "Wrong size");
	static_assert(sizeof(mvec3) == sizeof(MixedDataVector3), "Wrong size");
	static_assert(sizeof(pvec3) == sizeof(PaddedVector3), "Wrong size");

	//Vector Wrapper debug assertions
	static_assert(not implem::HasOffsetXEnum<VectorWrapperInfo<float>>::value, "Should return false");
	static_assert(implem::HasOffsetXEnum<VectorWrapperInfo<SimpleVector2>>::value, "Should return true");
	static_assert(implem::have_compat_offsets<SimpleVector3, PaddedVector3>::value, "Should be true");
	static_assert(implem::have_compat_layout<SimpleVector3, PaddedVector3>::value, "Should be true");
	static_assert(implem::is_vec<pvec3>::value, "Should be true");
	static_assert(implem::is_vec<svec3>::value, "Should be true");
} //namespace vwr

#endif
