/*
 * Copyright 2015 Michele "King_DuckZ" Santullo
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

#ifndef id1F562E68E5184552A7E8C9270B016692
#define id1F562E68E5184552A7E8C9270B016692

#include "vectorwrapper/has_method.hpp"
#include "vectorwrapper/sequence_bt.hpp"
#include <cstddef>
#include <ciso646>
#include <type_traits>
#include <array>
#include <cassert>

namespace vwr {
	template <typename V>
	struct VectorWrapperInfo;

	template <typename V, std::size_t S=VectorWrapperInfo<V>::dimensions>
	class Vec;

	namespace implem {
		define_has_typedef(lower_vector_type, LowerVec);
		define_has_typedef(higher_vector_type, HigherVec);
		define_has_enum(vector_x, VectorX);
		define_has_method(get_at, GetAt);

		template <typename V1, typename V2, std::size_t D>
		Vec<V1>& assign ( Vec<V1, D>& parLeft, const Vec<V2, D>& parRight );
		template <typename V>
		Vec<V>& assign_same_type ( Vec<V>& parLeft, const Vec<V>& parRight );

		template <typename T, typename U, std::size_t TS=VectorWrapperInfo<T>::dimensions, std::size_t US=VectorWrapperInfo<T>::dimensions> struct have_same_offsets {
			enum { value = false };
		};
		template <typename T, typename U> struct have_same_offsets<T, U, 1, 1> {
			enum {
				value = (VectorWrapperInfo<T>::offset_x == VectorWrapperInfo<U>::offset_x)
			};
		};
		template <typename T, typename U> struct have_same_offsets<T, U, 2, 2> {
			enum {
				value = (VectorWrapperInfo<T>::offset_x == VectorWrapperInfo<U>::offset_x) and
					(VectorWrapperInfo<T>::offset_y == VectorWrapperInfo<U>::offset_y)
			};
		};
		template <typename T, typename U> struct have_same_offsets<T, U, 3, 3> {
			enum {
				value = (VectorWrapperInfo<T>::offset_x == VectorWrapperInfo<U>::offset_x) and
					(VectorWrapperInfo<T>::offset_y == VectorWrapperInfo<U>::offset_y) and
					(VectorWrapperInfo<T>::offset_z == VectorWrapperInfo<U>::offset_z)
			};
		};
		template <typename T, typename U> struct have_same_offsets<T, U, 4, 4> {
			enum {
				value = (VectorWrapperInfo<T>::offset_x == VectorWrapperInfo<U>::offset_x) and
					(VectorWrapperInfo<T>::offset_y == VectorWrapperInfo<U>::offset_y) and
					(VectorWrapperInfo<T>::offset_z == VectorWrapperInfo<U>::offset_z) and
					(VectorWrapperInfo<T>::offset_w == VectorWrapperInfo<U>::offset_w)
			};
		};

		template <typename T, typename U>
		struct have_same_layout {
			enum {
				value =
					HasVectorXEnum<T>::value and HasVectorXEnum<U>::value and
					VectorWrapperInfo<T>::dimensions == VectorWrapperInfo<U>::dimensions and
					have_same_offsets<T, U>::value;
			};
		};

		template <typename V> struct is_vec {
			enum { value = false };
		};
		template <typename V> struct is_vec<Vec<V>> {
			enum { value = true };
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
			explicit VecBase ( scalar_type parInit );
			explicit VecBase ( const vector_type& parInit );
			template <typename... Args>
			VecBase ( scalar_type parX, scalar_type parY, Args... parArgs );
			~VecBase ( void ) = default;

			scalar_type& operator[] ( std::size_t parIndex );
			scalar_type& operator[] ( int parIndex );
			const scalar_type& operator[] ( std::size_t parIndex ) const;
			const scalar_type& operator[] ( int parIndex ) const;

			vector_type& data ( void ) { return m_wrapped; }
			const vector_type& data ( void ) const { return m_wrapped; }

			template <typename V2>
			const typename std::enable_if<is_vec<V2>::value and have_same_layout<V, typename V2::vector_type>::value, V2>::type& cast ( void ) const;
			template <typename V2>
			typename std::enable_if<is_vec<V2>::value and have_same_layout<V, typename V2::vector_type>::value, V2>::type& cast ( void );

			template <typename V2> VecBase& operator+= ( const VecBase<V2>& parOther );
			template <typename V2> VecBase& operator-= ( const VecBase<V2>& parOther );
			template <typename V2> VecBase& operator*= ( const VecBase<V2>& parOther );
			template <typename V2> VecBase& operator/= ( const VecBase<V2>& parOther );

		private:
			vector_type m_wrapped;
		};

		template <typename T, std::size_t I> struct get_offset_enum_from_index;
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

		template <typename T, std::size_t S=VectorWrapperInfo<T>::dimensions>
		struct offsets_array_wrapper {
			template <std::size_t... I>
			offsets_array_wrapper ( const bt::index_seq<I...>& );

			const std::array<unsigned int, S> offsets;
		};

		template <typename T, bool=HasVectorXEnum<VectorWrapperInfo<T>>::value and std::is_standard_layout<typename VectorWrapperInfo<T>::vector_type>::value>
		class VecGetter;
		template <typename T>
		struct VecGetter<T, true> {
			static typename VectorWrapperInfo<T>::scalar_type& get_at ( typename VectorWrapperInfo<T>::vector_type& parVec, std::size_t parIndex );
		};
		template <typename T>
		struct VecGetter<T, false> {
		private:
			static_assert(HasGetAtMethod<VectorWrapperInfo<T>>::value, "You must provide a get_at() static method for this vector_type");
			typedef decltype(&VectorWrapperInfo<T>::get_at) get_at_func;
			static_assert(not std::is_rvalue_reference<typename std::result_of<get_at_func>::type>::value, "rvalue ref return types not implemented");
			static_assert(std::is_lvalue_reference<typename std::result_of<get_at_func>::type>::value, "Read-only vectors not implemented");

		public:
			static typename VectorWrapperInfo<T>::scalar_type& get_at ( typename VectorWrapperInfo<T>::vector_type& parVec, std::size_t parIndex );
		};

		template <typename V, bool Enabled> struct Vec2Promotion;
		template <typename V> struct Vec2Promotion<V, false> {};
		template <typename V> struct Vec2Promotion<V, true> {
			typedef Vec<typename VectorWrapperInfo<V>::higher_vector_type> higher_vector_type;
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;

			static_assert(VectorWrapperInfo<typename VectorWrapperInfo<V>::higher_vector_type>::dimensions == 3, "Wrong promoted vector type");

			higher_vector_type xy1 ( void ) const { return xyz(scalar_type(1)); }
			higher_vector_type xy0 ( void ) const { return xyz(scalar_type(0)); }
			higher_vector_type xyz ( const scalar_type& parZ ) const;
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

		template <typename V, std::size_t D>
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
		};

		template <typename V>
		struct VecAccessors<V, 2> : Vec2Promotion<V, HasHigherVecTypedef<VectorWrapperInfo<V>>::value> {
			typedef typename VectorWrapperInfo<V>::scalar_type scalar_type;
			const Vec<V>& xy ( void ) const { return *static_cast<const Vec<V>*>(this); }
			const scalar_type& x ( void ) const;
			const scalar_type& y ( void ) const;
			scalar_type& x ( void );
			scalar_type& y ( void );
		};
	} //namespace implem

	template <typename V, std::size_t S>
	class Vec : public implem::VecBase<V> {
	public:
		enum {
			dimensions = S
		};
	};

	template <typename V>
	class Vec<V, 1> : public implem::VecBase<V> {
		typedef typename implem::VecBase<V>::vector_type vector_type;
		typedef typename implem::VecBase<V>::scalar_type scalar_type;
	public:
		enum {
			dimensions = 1
		};

		static const Vec<V, 1> unit_x;

		Vec ( void ) = default;
		Vec ( const Vec& ) = default;
		explicit Vec ( const vector_type& parIn ) : implem::VecBase<V>(parIn) { }
		explicit Vec ( const scalar_type parX ) : implem::VecBase<V>(parX) { }
		template <typename V2> Vec ( const Vec<V2, dimensions>& parOther ) { implem::assign(*this, parOther); }

		scalar_type& x ( void ) { return (*this)[0]; }
		const scalar_type& x ( void ) const { return (*this)[0]; }

		Vec& operator= ( const Vec& parOther ) { return implem::assign_same_type(*this, parOther); }
		template <typename V2>
		Vec& operator= ( const Vec<V2, dimensions>& parOther ) { return implem::assign(*this, parOther); }
	};

	template <typename V>
	class Vec<V, 2> : public implem::VecBase<V>, public implem::VecAccessors<V, 2> {
		static_assert(std::is_standard_layout<implem::VecBase<V>>::value, "Base class must be a standard layout type");
		static_assert(std::is_standard_layout<implem::VecAccessors<V, 2>>::value, "Base class must be a standard layout type");
		typedef typename implem::VecBase<V>::scalar_type scalar_type;
		typedef typename implem::VecBase<V>::vector_type vector_type;
	public:
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
		template <typename V2> Vec ( const Vec<V2, dimensions>& parOther ) { implem::assign(*this, parOther); }

		Vec& operator= ( const Vec& parOther ) { return implem::assign_same_type(*this, parOther); }
		template <typename V2>
		Vec& operator= ( const Vec<V2, dimensions>& parOther ) { return implem::assign(*this, parOther); }
		Vec& operator= ( const vector_type& parOther ) { this->data() = parOther; return *this; }
	};

	template <typename V>
	class Vec<V, 3> : public implem::VecBase<V>, public implem::VecAccessors<V, 3> {
		static_assert(std::is_standard_layout<implem::VecBase<V>>::value, "Base class must be a standard layout type");
		static_assert(std::is_standard_layout<implem::VecAccessors<V, 3>>::value, "Base class must be a standard layout type");
		typedef typename implem::VecBase<V>::scalar_type scalar_type;
		typedef typename implem::VecBase<V>::vector_type vector_type;
	public:
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
		template <typename V2> Vec ( const Vec<V2, dimensions>& parOther ) { implem::assign(*this, parOther); }

		Vec& operator= ( const Vec& parOther ) { return implem::assign_same_type(*this, parOther); }
		template <typename V2>
		Vec& operator= ( const Vec<V2, dimensions>& parOther ) { return implem::assign(*this, parOther); }
		Vec& operator= ( const vector_type& parOther ) { this->data() = parOther; return *this; }
	};

	template <typename V>
	Vec<V> mk_vec ( const V& parVec );

	template <typename V1, typename V2>
	bool operator== ( const Vec<V1>& parLeft, const Vec<V2>& parRight );
	template <typename V1, typename V2>
	bool operator< ( const Vec<V1>& parLeft, const Vec<V2>& parRight );

	template <typename V>
	bool operator== ( const Vec<V>& parLeft, const typename VectorWrapperInfo<V>::scalar_type& parRight );
	template <typename V>
	bool operator< ( const Vec<V>& parLeft, const typename VectorWrapperInfo<V>::scalar_type& parRight );

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
} //namespace vwr

#include "vectorwrapper/vectorwrapper.inl"

#endif