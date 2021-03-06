/*
 * Copyright 2015-2017 Michele "King_DuckZ" Santullo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "has_method.hpp"
#include "sequence_bt.hpp"
#include "size_type.hpp"
#include <ciso646>
#include <type_traits>
#include <array>
#include <cassert>

#if defined VWR_OUTER_NAMESPACE
namespace VWR_OUTER_NAMESPACE {
#endif

namespace vwr {
	template <typename V>
	struct VectorWrapperInfo;

	template <typename V, size_type S=VectorWrapperInfo<V>::dimensions>
	class Vec;

	namespace implem {
		define_has_typedef(lower_vector_type, LowerVec);
		define_has_typedef(higher_vector_type, HigherVec);
		define_has_enum(offset_x, OffsetX);
		define_has_method(get_at, GetAt);
		define_has_enum(cast_ignore_trailing_properties, CastIgnoreTrailingProperties);

#if defined(VWR_WITH_IMPLICIT_CONVERSIONS)
		template <typename V1, typename V2, size_type D>
		Vec<V1>& assign ( Vec<V1, D>& parLeft, const Vec<V2, D>& parRight );
#endif
		template <typename V>
		Vec<V>& assign_same_type ( Vec<V>& parLeft, const Vec<V>& parRight );

		template <typename T, size_type I> struct get_offset_enum_from_index;
		template <typename T> struct get_offset_enum_from_index<T, 0> {
			enum { value = VectorWrapperInfo<T>::offset_x };
		};
		template <typename T> struct get_offset_enum_from_index<T, 1> {
			enum { value = VectorWrapperInfo<T>::offset_y };
		};
		template <typename T> struct get_offset_enum_from_index<T, 2> {
			enum { value = VectorWrapperInfo<T>::offset_z };
		};
		template <typename T> struct get_offset_enum_from_index<T, 3> {
			enum { value = VectorWrapperInfo<T>::offset_w };
		};

		template <typename T, size_type S=VectorWrapperInfo<T>::dimensions> struct min_offset {
			enum {
				value = (
					static_cast<int>(get_offset_enum_from_index<T, S-1>::value) < static_cast<int>(min_offset<T, S-1>::value) ?
						static_cast<int>(get_offset_enum_from_index<T, S-1>::value) :
						static_cast<int>(min_offset<T, S-1>::value)
					)
			};
		};
		template <typename T> struct min_offset<T, 1> {
			enum { value = get_offset_enum_from_index<T, 0>::value };
		};

		template <
			typename T,
			typename U,
			size_type S=(
				static_cast<int>(VectorWrapperInfo<T>::dimensions) < static_cast<int>(VectorWrapperInfo<U>::dimensions) ?
					static_cast<int>(VectorWrapperInfo<T>::dimensions)
				:
					static_cast<int>(VectorWrapperInfo<U>::dimensions)
				)
			> struct have_compat_offsets;
		template <typename T, typename U> struct have_compat_offsets<T, U, 1> {
			enum {
				value = true
			};
		};
		template <typename T, typename U> struct have_compat_offsets<T, U, 2> {
			enum {
				value =
					VectorWrapperInfo<T>::offset_x - min_offset<T>::value == VectorWrapperInfo<U>::offset_x - min_offset<U>::value and
					VectorWrapperInfo<T>::offset_y - min_offset<T>::value == VectorWrapperInfo<U>::offset_y - min_offset<U>::value
			};
		};
		template <typename T, typename U> struct have_compat_offsets<T, U, 3> {
			enum {
				value =
					VectorWrapperInfo<T>::offset_x - min_offset<T>::value == VectorWrapperInfo<U>::offset_x - min_offset<U>::value and
					VectorWrapperInfo<T>::offset_y - min_offset<T>::value == VectorWrapperInfo<U>::offset_y - min_offset<U>::value and
					VectorWrapperInfo<T>::offset_z - min_offset<T>::value == VectorWrapperInfo<U>::offset_z - min_offset<U>::value
			};
		};
		template <typename T, typename U> struct have_compat_offsets<T, U, 4> {
			enum {
				value =
					VectorWrapperInfo<T>::offset_x - min_offset<T>::value == VectorWrapperInfo<U>::offset_x - min_offset<U>::value and
					VectorWrapperInfo<T>::offset_y - min_offset<T>::value == VectorWrapperInfo<U>::offset_y - min_offset<U>::value and
					VectorWrapperInfo<T>::offset_z - min_offset<T>::value == VectorWrapperInfo<U>::offset_z - min_offset<U>::value and
					VectorWrapperInfo<T>::offset_w - min_offset<T>::value == VectorWrapperInfo<U>::offset_w - min_offset<U>::value
			};
		};

		template <typename T, typename U>
		struct have_compat_layout {
			enum {
				value =
					HasOffsetXEnum<VectorWrapperInfo<T>>::value and
					HasOffsetXEnum<VectorWrapperInfo<U>>::value and
					have_compat_offsets<T, U>::value
			};
		};

		template <typename V> struct is_vec {
			enum { value = false };
		};
		template <typename V> struct is_vec<Vec<V>> {
			typedef V vector_type;
			enum { value = true };
		};

		template <typename V> struct get_wrapped_ifn {
			typedef V type;
		};
		template <typename T> struct get_wrapped_ifn<Vec<T>> {
			typedef T type;
		};

		template <typename V1, typename V2>
		struct directly_convertible {
			enum {
				value =
					not HasGetAtMethod<VectorWrapperInfo<typename get_wrapped_ifn<V1>::type>>::value and
					not HasGetAtMethod<VectorWrapperInfo<typename get_wrapped_ifn<V2>::type>>::value
			};
		};

		template <typename V>
		class VecBase {
			friend Vec<V>& assign_same_type<V> ( Vec<V>& parLeft, const Vec<V>& parRight );
		public:
			typedef V vector_type;
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;

			enum {
				dimensions = VectorWrapperInfo<V>::dimensions
			};

			VecBase ( void ) = default;
			template <typename T>
			explicit VecBase ( const T& parInit, typename std::enable_if<std::is_same<T, scalar_type>::value and not std::is_same<scalar_type, vector_type>::value, bool>::type=false );
			explicit VecBase ( const vector_type& parInit );
			template <typename... Args>
			VecBase ( scalar_type parX, scalar_type parY, Args... parArgs );
			~VecBase ( void ) = default;

			scalar_type& operator[] ( size_type parIndex );
			const scalar_type& operator[] ( size_type parIndex ) const;

			vector_type& data ( void ) { return m_wrapped; }
			const vector_type& data ( void ) const { return m_wrapped; }

			template <typename V2>
			const typename std::enable_if<is_vec<V2>::value and directly_convertible<V, V2>::value, V2>::type& cast ( void ) const;
			template <typename V2>
			typename std::enable_if<is_vec<V2>::value and directly_convertible<V, V2>::value, V2>::type& cast ( void );

			template <typename V2>
			const typename std::enable_if<is_vec<V2>::value and not directly_convertible<V, V2>::value, V2>::type& cast ( void ) const;
			template <typename V2>
			typename std::enable_if<is_vec<V2>::value and not directly_convertible<V, V2>::value, V2>::type& cast ( void );

			template <typename V2> VecBase& operator+= ( const VecBase<V2>& parOther );
			template <typename V2> VecBase& operator-= ( const VecBase<V2>& parOther );
			template <typename V2> VecBase& operator*= ( const VecBase<V2>& parOther );
			template <typename V2> VecBase& operator/= ( const VecBase<V2>& parOther );
			VecBase& operator+= ( const scalar_type& parOther );
			VecBase& operator-= ( const scalar_type& parOther );
			VecBase& operator*= ( const scalar_type& parOther );
			VecBase& operator/= ( const scalar_type& parOther );

		private:
			template <size_type... I, typename... Args>
			void assign_values (const bt::number_seq<size_type, I...>&, Args... parArgs);
			template <typename Op, typename V2, size_type... I>
			void assign_values_op (Op parOp, const bt::number_seq<size_type, I...>& parSeq, const VecBase<V2>& parOther);
			template <typename Op, size_type... I>
			void assign_values_op_scalar (Op parOp, const bt::number_seq<size_type, I...>& parSeq, const scalar_type& parOther);

			vector_type m_wrapped;
		};

		template <typename T, size_type S=VectorWrapperInfo<T>::dimensions>
		struct offsets_array_wrapper {
			template <size_type... I>
			offsets_array_wrapper ( const bt::number_seq<size_type, I...>& );

			const std::array<unsigned int, S> offsets;
		};

		template <typename T, bool=HasOffsetXEnum<VectorWrapperInfo<T>>::value and std::is_standard_layout<T>::value>
		struct VecGetter;
		template <typename T>
		struct VecGetter<T, true> {
			static typename VectorWrapperInfo<T>::scalar_type& get_at ( T& parVec, size_type parIndex );
		};
		template <typename T>
		struct VecGetter<T, false> {
		private:
			static_assert(HasGetAtMethod<VectorWrapperInfo<T>>::value, "You must provide a get_at() static method for this vector_type");
			typedef typename VectorWrapperInfo<T>::scalar_type scalar_type;
			typedef T vector_type;
			using get_at_func = decltype(&VectorWrapperInfo<T>::get_at)(size_type, vector_type&);
			using get_at_rettype = typename std::result_of<get_at_func>::type;

			static_assert(not std::is_rvalue_reference<get_at_rettype>::value, "rvalue ref return types not implemented");
			static_assert(std::is_lvalue_reference<get_at_rettype>::value, "Read-only vectors not implemented");

		public:
			static get_at_rettype get_at ( T& parVec, size_type parIndex );
		};

		template <typename V, bool Enabled> struct Vec1Promotion;
		template <typename V> struct Vec1Promotion<V, false> { };
		template <typename V> struct Vec1Promotion<V, true> {
			typedef Vec<typename VectorWrapperInfo<V>::higher_vector_type> higher_vector_type;
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;

			static_assert(VectorWrapperInfo<typename VectorWrapperInfo<V>::higher_vector_type>::dimensions == 2, "Wrong promoted vector type");

			higher_vector_type x1 ( void ) const { return xn(scalar_type(1)); }
			higher_vector_type x0 ( void ) const { return xn(scalar_type(0)); }
			higher_vector_type xn ( const scalar_type& parN ) const;
			higher_vector_type nx ( const scalar_type& parN ) const;
		};

		template <typename V, bool Enabled> struct Vec2Promotion;
		template <typename V> struct Vec2Promotion<V, false> {};
		template <typename V> struct Vec2Promotion<V, true> {
			typedef Vec<typename VectorWrapperInfo<V>::higher_vector_type> higher_vector_type;
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;

			static_assert(VectorWrapperInfo<typename VectorWrapperInfo<V>::higher_vector_type>::dimensions == 3, "Wrong promoted vector type");

			higher_vector_type xy1 ( void ) const { return xyn(scalar_type(1)); }
			higher_vector_type xy0 ( void ) const { return xyn(scalar_type(0)); }
			higher_vector_type xyn ( const scalar_type& parZ ) const;
		};

		template <typename V, bool Enabled> struct Vec3Promotion;
		template <typename V> struct Vec3Promotion<V, false> {};
		template <typename V> struct Vec3Promotion<V, true> {
			typedef Vec<typename VectorWrapperInfo<V>::higher_vector_type> higher_vector_type;
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;

			static_assert(VectorWrapperInfo<typename VectorWrapperInfo<V>::higher_vector_type>::dimensions == 4, "Wrong promoted vector type");

			higher_vector_type xyz1 ( void ) const { return xyzw(scalar_type(1)); }
			higher_vector_type xyz0 ( void ) const { return xyzw(scalar_type(0)); }
			higher_vector_type xyzw ( const scalar_type& parW ) const;
		};

		template <typename V, bool Enabled> struct Vec3Demotion;
		template <typename V> struct Vec3Demotion<V, false> : Vec3Promotion<V, HasHigherVecTypedef<V>::value> {};
		template <typename V> struct Vec3Demotion<V, true> : Vec3Promotion<V, HasHigherVecTypedef<V>::value> {
			typedef Vec<typename VectorWrapperInfo<V>::lower_vector_type> lower_vector_type;
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;

			static_assert(VectorWrapperInfo<typename VectorWrapperInfo<V>::lower_vector_type>::dimensions == 2, "Wrong demoted vector type");

			lower_vector_type xy ( void ) const;
			lower_vector_type xz ( void ) const;
			lower_vector_type yz ( void ) const;
		};

		template <typename V, size_type D>
		struct VecAccessors;

		//Workaround for visual studio - VecAccessors<V, 3> should inherit from
		//both Vec3Promotion and Vec3Demotion, but when I do that, sizeof(Vec)
		//is wrong. I had to linearize the inheritance hierarchy so that the
		//sizeof(Vec<V>) == sizeof(V) condition is verified.
		template <typename V>
		struct VecAccessors<V, 3> : Vec3Demotion<V, HasLowerVecTypedef<VectorWrapperInfo<V>>::value> {
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;
			const Vec<V>& xyz ( void ) const { return *static_cast<const Vec<V>*>(this); }
			const scalar_type& x ( void ) const;
			const scalar_type& y ( void ) const;
			const scalar_type& z ( void ) const;
			scalar_type& x ( void );
			scalar_type& y ( void );
			scalar_type& z ( void );

#if defined(VWR_EXTRA_ACCESSORS)
			Vec<V> xy0 ( void ) const { return Vec<V>(x(), y(), scalar_type(0)); }
			Vec<V> xy1 ( void ) const { return Vec<V>(x(), y(), scalar_type(1)); }
			Vec<V> x0z ( void ) const { return Vec<V>(x(), scalar_type(0), z()); }
			Vec<V> x1z ( void ) const { return Vec<V>(x(), scalar_type(1), z()); }
			Vec<V> _0yz ( void ) const { return Vec<V>(scalar_type(0), y(), z()); }
			Vec<V> _1yz ( void ) const { return Vec<V>(scalar_type(1), y(), z()); }
			Vec<V> zxy ( void ) const { return Vec<V>(z(), x(), y()); }
#endif
		};

		template <typename V>
		struct VecAccessors<V, 2> : Vec2Promotion<V, HasHigherVecTypedef<VectorWrapperInfo<V>>::value> {
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;
			const Vec<V>& xy ( void ) const { return *static_cast<const Vec<V>*>(this); }
			const scalar_type& x ( void ) const;
			const scalar_type& y ( void ) const;
			scalar_type& x ( void );
			scalar_type& y ( void );

#if defined(VWR_EXTRA_ACCESSORS)
			Vec<V> x0 ( void ) const { return Vec<V>(x(), scalar_type(0)); }
			Vec<V> x1 ( void ) const { return Vec<V>(x(), scalar_type(1)); }
			Vec<V> yx ( void ) const { return Vec<V>(y(), x()); }
			Vec<V> _0y ( void ) const { return Vec<V>(scalar_type(0), y()); }
			Vec<V> _1y ( void ) const { return Vec<V>(scalar_type(1), y()); }
#endif
		};

		template <typename V>
		struct VecAccessors<V, 1> : Vec1Promotion<V, HasHigherVecTypedef<VectorWrapperInfo<V>>::value> {
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;
			scalar_type& x ( void );
			const scalar_type& x ( void ) const;
		};

		template <bool LastVal, typename V1, typename V2, typename ComposeOp, typename Op>
		constexpr bool compare ( const Vec<V1>& parLeft, const Vec<V2>& parRight, Op parComposeOp, Op parOp, bt::number_seq<size_type> );
		template <bool LastVal, typename V1, typename V2, typename ComposeOp, typename Op, size_type I1, size_type... I>
		bool compare ( const Vec<V1>& parLeft, const Vec<V2>& parRight, ComposeOp parComposeOp, Op parOp, bt::number_seq<size_type, I1, I...> );

		template <typename V1, typename V2, typename Op, size_type... I>
		Vec<typename std::common_type<V1, V2>::type> binary_op ( const Vec<V1>& parLeft, const Vec<V2>& parRight, Op parOp, const Vec<typename std::common_type<V1, V2>::type>& parLastVal, bt::number_seq<size_type, I...> );
	} //namespace implem

	template <typename V> struct is_castable_to { };

	template <typename V, size_type S>
	class Vec : public implem::VecBase<V> {
	public:
		enum {
			dimensions = S
		};
	};

	template <typename V>
	class Vec<V, 1> : public implem::VecBase<V>, public implem::VecAccessors<V, 1> {
		static_assert(std::is_standard_layout<implem::VecBase<V>>::value, "Base class must be a standard layout type");
	public:
		typedef typename implem::VecBase<V>::vector_type vector_type;
		typedef typename implem::VecBase<V>::scalar_type scalar_type;
		enum {
			dimensions = 1
		};

		static const Vec<V, 1> unit_x;

		Vec ( void ) = default;
		Vec ( const Vec& ) = default;
		explicit Vec ( const vector_type& parIn ) : implem::VecBase<V>(parIn) { }
		template <typename T>
		explicit Vec ( const typename std::enable_if<std::is_same<T, scalar_type>::value and not std::is_same<scalar_type, vector_type>::value, T>::type& parX ) : implem::VecBase<V>(parX) { }
#if defined(VWR_WITH_IMPLICIT_CONVERSIONS)
		template <typename V2> Vec ( const Vec<V2, dimensions>& parOther ) { implem::assign(*this, parOther); }
		template <typename V2>
		Vec& operator= ( const Vec<V2, dimensions>& parOther ) { return implem::assign(*this, parOther); }
#endif

		Vec& operator= ( const Vec& parOther ) { return implem::assign_same_type(*this, parOther); }
	};

	template <typename V>
	class Vec<V, 2> : public implem::VecBase<V>, public implem::VecAccessors<V, 2> {
		static_assert(std::is_standard_layout<implem::VecBase<V>>::value, "Base class must be a standard layout type");
		static_assert(std::is_standard_layout<implem::VecAccessors<V, 2>>::value, "Base class must be a standard layout type");
	public:
		typedef typename implem::VecBase<V>::scalar_type scalar_type;
		typedef typename implem::VecBase<V>::vector_type vector_type;
		enum {
			dimensions = 2
		};

		static const Vec<V, 2> unit_x;
		static const Vec<V, 2> unit_y;

		Vec ( void ) = default;
		Vec ( const Vec& ) = default;
		explicit Vec ( const vector_type& parIn ) : implem::VecBase<V>(parIn) { }
		explicit Vec ( const scalar_type parX ) : implem::VecBase<V>(parX) { }
		Vec ( scalar_type parX, scalar_type parY ) : implem::VecBase<V>(parX, parY) { }
#if defined(VWR_WITH_IMPLICIT_CONVERSIONS)
		template <typename V2> Vec ( const Vec<V2, dimensions>& parOther ) { implem::assign(*this, parOther); }
		template <typename V2>
		Vec& operator= ( const Vec<V2, dimensions>& parOther ) { return implem::assign(*this, parOther); }
#endif

		Vec& operator= ( const Vec& parOther ) { return implem::assign_same_type(*this, parOther); }
		Vec& operator= ( const vector_type& parOther ) { this->data() = parOther; return *this; }
	};

	template <typename V>
	class Vec<V, 3> : public implem::VecBase<V>, public implem::VecAccessors<V, 3> {
		static_assert(std::is_standard_layout<implem::VecBase<V>>::value, "Base class must be a standard layout type");
		static_assert(std::is_standard_layout<implem::VecAccessors<V, 3>>::value, "Base class must be a standard layout type");
	public:
		typedef typename implem::VecBase<V>::scalar_type scalar_type;
		typedef typename implem::VecBase<V>::vector_type vector_type;
		enum {
			dimensions = 3
		};

		static const Vec<V, 3> unit_x;
		static const Vec<V, 3> unit_y;
		static const Vec<V, 3> unit_z;

		Vec ( void ) = default;
		Vec ( const Vec& ) = default;
		explicit Vec ( const vector_type& parIn ) : implem::VecBase<V>(parIn) { }
		explicit Vec ( const scalar_type parX ) : implem::VecBase<V>(parX) { }
		Vec ( scalar_type parX, scalar_type parY, scalar_type parZ ) : implem::VecBase<V>(parX, parY, parZ) { }
#if defined(VWR_WITH_IMPLICIT_CONVERSIONS)
		template <typename V2> Vec ( const Vec<V2, dimensions>& parOther ) { implem::assign(*this, parOther); }
		template <typename V2>
		Vec& operator= ( const Vec<V2, dimensions>& parOther ) { return implem::assign(*this, parOther); }
#endif

		Vec& operator= ( const Vec& parOther ) { return implem::assign_same_type(*this, parOther); }
		Vec& operator= ( const vector_type& parOther ) { this->data() = parOther; return *this; }
	};

	template <typename V>
	Vec<V> mk_vec ( const V& parVec );

	template <typename V1, typename V2>
	bool operator== ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	bool operator!= ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	bool operator< ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	bool operator> ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	bool operator<= ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	bool operator>= ( const Vec<V1>& parLeft, const Vec<V2>& parRight );

	template <typename V>
	bool operator== ( const Vec<V>& parLeft, const typename VectorWrapperInfo<V>::scalar_type& parRight );
	template <typename V>
	bool operator< ( const Vec<V>& parLeft, const typename VectorWrapperInfo<V>::scalar_type& parRight );
	template <typename V>
	Vec<V> operator- ( const Vec<V>& parVec );

	template <typename V1, typename T>
	bool operator> ( const Vec<V1>& parLeft, const T& parRight );
	template <typename V1, typename T>
	bool operator<= ( const Vec<V1>& parLeft, const T& parRight );
	template <typename V1, typename T>
	bool operator>= ( const Vec<V1>& parLeft, const T& parRight );
	template <typename V1, typename T>
	bool operator!= ( const Vec<V1>& parLeft, const T& parRight );

	template <typename V1, typename V2>
	Vec<typename std::common_type<V1, V2>::type> operator+ ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	Vec<typename std::common_type<V1, V2>::type> operator- ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	Vec<typename std::common_type<V1, V2>::type> operator* ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	Vec<typename std::common_type<V1, V2>::type> operator/ ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	Vec<typename std::common_type<V1, V2>::type> operator% ( const Vec<V1>& parLeft, const Vec<V2>& parRight );

	template <typename V>
	Vec<V> operator+ ( const Vec<V>& parLeft, const typename Vec<V>::scalar_type& parRight );
	template <typename V>
	Vec<V> operator- ( const Vec<V>& parLeft, const typename Vec<V>::scalar_type& parRight );
	template <typename V>
	Vec<V> operator* ( const Vec<V>& parLeft, const typename Vec<V>::scalar_type& parRight );
	template <typename V>
	Vec<V> operator/ ( const Vec<V>& parLeft, const typename Vec<V>::scalar_type& parRight );
	template <typename V>
	Vec<V> operator% ( const Vec<V>& parLeft, const typename Vec<V>::scalar_type& parRight );
	template <typename V>
	Vec<V> operator+ ( const typename Vec<V>::scalar_type& parLeft, const Vec<V>& parRight );
	template <typename V>
	Vec<V> operator- ( const typename Vec<V>::scalar_type& parLeft, const Vec<V>& parRight );
	template <typename V>
	Vec<V> operator* ( const typename Vec<V>::scalar_type& parLeft, const Vec<V>& parRight );
	template <typename V>
	Vec<V> operator/ ( const typename Vec<V>::scalar_type& parLeft, const Vec<V>& parRight );
	template <typename V>
	Vec<V> operator% ( const typename Vec<V>::scalar_type& parLeft, const Vec<V>& parRight );
} //namespace vwr

#if defined VWR_OUTER_NAMESPACE
} //namespace VWR_OUTER_NAMESPACE
#endif

#include "vectorwrapper.inl"
