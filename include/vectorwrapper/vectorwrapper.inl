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

namespace vwr {
	namespace implem {
		template <typename T, bool=HasCastIgnoreTrailingPropertiesEnum<VectorWrapperInfo<T>>::value> struct IsCastIgnoreTrailingPropertiesSet;
		template <typename T> struct IsCastIgnoreTrailingPropertiesSet<T, true> { static const bool value = static_cast<bool>(VectorWrapperInfo<T>::cast_ignore_trailing_properties); };
		template <typename T> struct IsCastIgnoreTrailingPropertiesSet<T, false> { static const bool value = false; };

		template <typename V>
		template <typename T>
		VecBase<V>::VecBase (const T& parInit, typename std::enable_if<std::is_same<T, scalar_type>::value and not std::is_same<scalar_type, vector_type>::value, bool>::type) {
			for (int z = 0; z < VectorWrapperInfo<V>::dimensions; ++z) {
				VecGetter<V>::get_at(m_wrapped, z) = parInit;
			}
		}

		template <typename V>
		VecBase<V>::VecBase (const vector_type& parInit) :
			m_wrapped(parInit)
		{
		}

		template <typename V>
		template <typename... Args>
		VecBase<V>::VecBase (scalar_type parX, scalar_type parY, Args... parArgs) {
			static_assert(2 + sizeof...(Args) == dimensions, "Wrong number of parameters received");
			VecGetter<V>::get_at(m_wrapped, 0) = parX;
			VecGetter<V>::get_at(m_wrapped, 1) = parY;

			assign_values(bt::index_range<2, dimensions>(), std::forward<Args>(parArgs)...);
		}

		template <typename V>
		template <std::size_t... I, typename... Args>
		void VecBase<V>::assign_values (const bt::index_seq<I...>&, Args... parArgs) {
			static_assert(sizeof...(I) == sizeof...(Args), "Argument count and indices count mismatch");

			std::initializer_list<scalar_type> t {
				(VecGetter<V>::get_at(m_wrapped, I) = parArgs)...
			};
			static_cast<void>(t);
		}

		template <typename V>
		auto VecBase<V>::operator[] (std::size_t parIndex) -> scalar_type& {
			return VecGetter<V>::get_at(m_wrapped, parIndex);
		}

		template <typename V>
		auto VecBase<V>::operator[] (int parIndex) -> scalar_type& {
			assert(parIndex >= 0);
			return VecGetter<V>::get_at(m_wrapped, static_cast<std::size_t>(parIndex));
		}

		template <typename V>
		auto VecBase<V>::operator[] (std::size_t parIndex) const -> const scalar_type& {
			return VecGetter<V>::get_at(const_cast<vector_type&>(m_wrapped), parIndex);
		}

		template <typename V>
		auto VecBase<V>::operator[] (int parIndex) const -> const scalar_type& {
			assert(parIndex >= 0);
			return VecGetter<V>::get_at(const_cast<vector_type&>(m_wrapped), static_cast<std::size_t>(parIndex));
		}

		template <typename V>
		template <typename V2>
		const typename std::enable_if<is_vec<V2>::value, V2>::type& VecBase<V>::cast() const {
			static_assert(sizeof(V2) <= sizeof(VecBase<V>) - min_offset<V>::value, "V2 must fit in V starting from the first coordinate");
			static_assert(std::is_standard_layout<V2>::value, "V2 must be a standard layout type");
			static_assert(min_offset<typename is_vec<V2>::vector_type>::value == 0, "V2 must not have any properties before the first coordinate");
			static_assert(have_compat_layout<V, typename is_vec<V2>::vector_type>::value, "V is not suitable for casting to V2");

			//Assert that V2 won't stomp on part of V's data, unless the user
			//has explicitly said he doesn't care.
			static_assert((sizeof(typename VectorWrapperInfo<typename is_vec<V2>::vector_type>::scalar_type) * VectorWrapperInfo<typename is_vec<V2>::vector_type>::dimensions == sizeof(V2)) or
				IsCastIgnoreTrailingPropertiesSet<typename is_vec<V2>::vector_type>::value,
				"V2 must not have any properties past the last coordinate");
			static_assert(alignof(typename VectorWrapperInfo<V>::scalar_type) == alignof(V2), "Casting to V2 would give you a misaligned variable");

			return *reinterpret_cast<const V2*>(
				reinterpret_cast<const char*>(this) + VectorWrapperInfo<V>::offset_x
			);
		}

		template <typename V>
		template <typename V2>
		typename std::enable_if<is_vec<V2>::value, V2>::type& VecBase<V>::cast() {
			return const_cast<V2&>(const_cast<const VecBase<V>*>(this)->cast<V2>());
		}

		template <typename V>
		template <typename V2>
		VecBase<V>& VecBase<V>::operator+= (const VecBase<V2>& parOther) {
			static_assert(static_cast<int>(VectorWrapperInfo<V>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
			for (int z = 0; z < VectorWrapperInfo<V>::dimensions; ++z) {
				(*this)[z] += parOther[z];
			}
			return *this;
		}
		template <typename V>
		template <typename V2>
		VecBase<V>& VecBase<V>::operator-= (const VecBase<V2>& parOther) {
			static_assert(static_cast<int>(VectorWrapperInfo<V>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
			for (int z = 0; z < VectorWrapperInfo<V>::dimensions; ++z) {
				(*this)[z] -= parOther[z];
			}
			return *this;
		}
		template <typename V>
		template <typename V2>
		VecBase<V>& VecBase<V>::operator*= (const VecBase<V2>& parOther) {
			static_assert(static_cast<int>(VectorWrapperInfo<V>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
			for (int z = 0; z < VectorWrapperInfo<V>::dimensions; ++z) {
				(*this)[z] *= parOther[z];
			}
			return *this;
		}
		template <typename V>
		template <typename V2>
		VecBase<V>& VecBase<V>::operator/= (const VecBase<V2>& parOther) {
			static_assert(static_cast<int>(VectorWrapperInfo<V>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
			for (int z = 0; z < VectorWrapperInfo<V>::dimensions; ++z) {
				(*this)[z] /= parOther[z];
			}
			return *this;
		}

		template <typename T>
		typename VectorWrapperInfo<T>::scalar_type& VecGetter<T, true>::get_at (T& parVec, std::size_t parIndex) {
			assert(parIndex < VectorWrapperInfo<T>::dimensions);
			typedef T vector_type;
			typedef typename VectorWrapperInfo<T>::scalar_type scalar_type;
			static_assert(std::is_standard_layout<vector_type>::value, "Can't use this function with this vector_type");
			const offsets_array_wrapper<T> oaw((bt::index_range<0, VectorWrapperInfo<T>::dimensions>()));
			return *reinterpret_cast<scalar_type*>(reinterpret_cast<char*>(&parVec) + oaw.offsets[parIndex]);
		}

		template <typename T>
		auto VecGetter<T, false>::get_at (T& parVec, std::size_t parIndex) -> get_at_rettype {
			assert(parIndex < VectorWrapperInfo<T>::dimensions);
			return VectorWrapperInfo<T>::get_at(parIndex, parVec);
		}

		template <typename V1, typename V2, std::size_t D>
		inline Vec<V1>& assign (Vec<V1, D>& parLeft, const Vec<V2, D>& parRight) {
			for (std::size_t z = 0; z < D; ++z) {
				parLeft[z] = parRight[z];
			}
			return parLeft;
		}

		template <typename V>
		Vec<V>& assign_same_type (Vec<V>& parLeft, const Vec<V>& parRight) {
			parLeft.m_wrapped = parRight.m_wrapped;
			return parLeft;
		}

		template <typename V>
		auto Vec3Demotion<V, true>::xy() const -> lower_vector_type {
			auto& this_vec = *static_cast<const Vec<V>*>(this);
			return lower_vector_type(this_vec[0], this_vec[1]);
		}

		template <typename V>
		auto Vec3Demotion<V, true>::xz() const -> lower_vector_type {
			auto& this_vec = *static_cast<const Vec<V>*>(this);
			return lower_vector_type(this_vec[0], this_vec[2]);
		}

		template <typename V>
		auto Vec3Demotion<V, true>::yz() const -> lower_vector_type {
			auto& this_vec = *static_cast<const Vec<V>*>(this);
			return lower_vector_type(this_vec[1], this_vec[2]);
		}

		template <typename V>
		auto Vec1Promotion<V, true>::xn (const scalar_type& parN) const -> higher_vector_type {
			auto& this_vec = *static_cast<const Vec<V>*>(this);
			return higher_vector_type(this_vec[0], parN);
		}

		template <typename V>
		auto Vec1Promotion<V, true>::nx (const scalar_type& parN) const -> higher_vector_type {
			auto& this_vec = *static_cast<const Vec<V>*>(this);
			return higher_vector_type(parN, this_vec[0]);
		}

		template <typename V>
		auto Vec2Promotion<V, true>::xyn (const scalar_type& parN) const -> higher_vector_type {
			auto& this_vec = *static_cast<const Vec<V>*>(this);
			return higher_vector_type(this_vec[0], this_vec[1], parN);
		}

		template <typename V>
		auto Vec3Promotion<V, true>::xyzw (const scalar_type& parW) const -> higher_vector_type {
			auto& this_vec = *static_cast<const Vec<V>*>(this);
			return higher_vector_type(this_vec[0], this_vec[1], this_vec[2], parW);
		}

		template <typename V>
		auto VecAccessors<V, 1>::x() -> scalar_type& {
			auto& this_vec = *static_cast<Vec<V>*>(this);
			return this_vec[0];
		}

		template <typename V>
		auto VecAccessors<V, 1>::x() const -> const scalar_type& {
			const auto& this_vec = *static_cast<const Vec<V>*>(this);
			return this_vec[0];
		}

		template <typename V>
		auto VecAccessors<V, 2>::x() -> scalar_type& {
			auto& this_vec = *static_cast<Vec<V>*>(this);
			return this_vec[0];
		}

		template <typename V>
		auto VecAccessors<V, 2>::x() const -> const scalar_type& {
			const auto& this_vec = *static_cast<const Vec<V>*>(this);
			return this_vec[0];
		}

		template <typename V>
		auto VecAccessors<V, 2>::y() -> scalar_type& {
			auto& this_vec = *static_cast<Vec<V>*>(this);
			return this_vec[1];
		}

		template <typename V>
		auto VecAccessors<V, 2>::y() const -> const scalar_type& {
			const auto& this_vec = *static_cast<const Vec<V>*>(this);
			return this_vec[1];
		}

		template <typename V>
		auto VecAccessors<V, 3>::x() -> scalar_type& {
			auto& this_vec = *static_cast<Vec<V>*>(this);
			return this_vec[0];
		}

		template <typename V>
		auto VecAccessors<V, 3>::x() const -> const scalar_type& {
			const auto& this_vec = *static_cast<const Vec<V>*>(this);
			return this_vec[0];
		}

		template <typename V>
		auto VecAccessors<V, 3>::y() -> scalar_type& {
			auto& this_vec = *static_cast<Vec<V>*>(this);
			return this_vec[1];
		}

		template <typename V>
		auto VecAccessors<V, 3>::y() const -> const scalar_type& {
			const auto& this_vec = *static_cast<const Vec<V>*>(this);
			return this_vec[1];
		}

		template <typename V>
		auto VecAccessors<V, 3>::z() -> scalar_type& {
			auto& this_vec = *static_cast<Vec<V>*>(this);
			return this_vec[2];
		}

		template <typename V>
		auto VecAccessors<V, 3>::z() const -> const scalar_type& {
			const auto& this_vec = *static_cast<const Vec<V>*>(this);
			return this_vec[2];
		}

		template <typename T, std::size_t S>
		template <std::size_t... I>
		offsets_array_wrapper<T, S>::offsets_array_wrapper (const bt::index_seq<I...>&) :
			offsets({get_offset_enum_from_index<T, I>::value...})
		{
			static_assert(sizeof...(I) == S, "Bug?");
		}
	} //namespace implem

	template <typename V> const Vec<V, 1> Vec<V, 1>::unit_x(scalar_type(1));
	template <typename V> const Vec<V, 2> Vec<V, 2>::unit_x(scalar_type(1), scalar_type(0));
	template <typename V> const Vec<V, 2> Vec<V, 2>::unit_y(scalar_type(0), scalar_type(1));
	template <typename V> const Vec<V, 3> Vec<V, 3>::unit_x(scalar_type(1), scalar_type(0), scalar_type(0));
	template <typename V> const Vec<V, 3> Vec<V, 3>::unit_y(scalar_type(0), scalar_type(1), scalar_type(0));
	template <typename V> const Vec<V, 3> Vec<V, 3>::unit_z(scalar_type(0), scalar_type(0), scalar_type(1));

	template <typename V>
	Vec<V> mk_vec (const V& parVec) {
		return Vec<V>(parVec);
	}

	template <typename V1, typename V2>
	inline bool operator== (const Vec<V1>& parLeft, const Vec<V2>& parRight) {
		static_assert(static_cast<int>(VectorWrapperInfo<V1>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
		bool retval = true;
		for (int z = 0; z < VectorWrapperInfo<V1>::dimensions; ++z) {
			retval &= (parLeft[z] == parRight[z]);
		}
		return retval;
	}
	template <typename V1, typename V2>
	inline bool operator< (const Vec<V1>& parLeft, const Vec<V2>& parRight) {
		static_assert(static_cast<int>(VectorWrapperInfo<V1>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
		bool retval = true;
		for (int z = 0; z < VectorWrapperInfo<V1>::dimensions; ++z) {
			retval &= (parLeft[z] < parRight[z]);
		}
		return retval;
	}

	template <typename V>
	inline bool operator== (const Vec<V>& parLeft, const typename VectorWrapperInfo<V>::scalar_type& parRight) {
		bool retval = true;
		for (int z = 0; z < VectorWrapperInfo<V>::dimensions; ++z) {
			retval &= (parLeft[z] == parRight);
		}
		return retval;
	}
	template <typename V>
	inline bool operator< ( const Vec<V>& parLeft, const typename VectorWrapperInfo<V>::scalar_type& parRight) {
		bool retval = true;
		for (int z = 0; z < VectorWrapperInfo<V>::dimensions; ++z) {
			retval &= (parLeft[z] < parRight);
		}
		return retval;
	}

	template <typename V1, typename T>
	inline bool operator> (const Vec<V1>& parLeft, const T& parRight) {
		return not (parLeft < parRight) and not (parLeft == parRight);
	}
	template <typename V1, typename T>
	inline bool operator<= (const Vec<V1>& parLeft, const T& parRight) {
		return (parLeft < parRight) or (parLeft == parRight);
	}
	template <typename V1, typename T>
	inline bool operator>= (const Vec<V1>& parLeft, const T& parRight) {
		return not (parLeft < parRight);
	}
	template <typename V1, typename T>
	inline bool operator!= (const Vec<V1>& parLeft, const T& parRight) {
		return not (parLeft == parRight);
	}

	template <typename V1, typename V2>
	inline Vec<typename std::common_type<V1, V2>::type> operator+ (const Vec<V1>& parLeft, const Vec<V2>& parRight) {
		static_assert(static_cast<int>(VectorWrapperInfo<V1>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
		Vec<typename std::common_type<V1, V2>::type> retval;
		for (int z = 0; z < VectorWrapperInfo<V1>::dimensions; ++z) {
			retval[z] = parLeft[z] + parRight[z];
		}
		return retval;
	}
	template <typename V1, typename V2>
	inline Vec<typename std::common_type<V1, V2>::type> operator- (const Vec<V1>& parLeft, const Vec<V2>& parRight) {
		static_assert(static_cast<int>(VectorWrapperInfo<V1>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
		Vec<typename std::common_type<V1, V2>::type> retval;
		for (int z = 0; z < VectorWrapperInfo<V1>::dimensions; ++z) {
			retval[z] = parLeft[z] - parRight[z];
		}
		return retval;
	}
	template <typename V1, typename V2>
	inline Vec<typename std::common_type<V1, V2>::type> operator* (const Vec<V1>& parLeft, const Vec<V2>& parRight) {
		static_assert(static_cast<int>(VectorWrapperInfo<V1>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
		Vec<typename std::common_type<V1, V2>::type> retval;
		for (int z = 0; z < VectorWrapperInfo<V1>::dimensions; ++z) {
			retval[z] = parLeft[z] * parRight[z];
		}
		return retval;
	}
	template <typename V1, typename V2>
	inline Vec<typename std::common_type<V1, V2>::type> operator/ (const Vec<V1>& parLeft, const Vec<V2>& parRight) {
		static_assert(static_cast<int>(VectorWrapperInfo<V1>::dimensions) == static_cast<int>(VectorWrapperInfo<V2>::dimensions), "Dimensions mismatch");
		Vec<typename std::common_type<V1, V2>::type> retval;
		for (int z = 0; z < VectorWrapperInfo<V1>::dimensions; ++z) {
			retval[z] = parLeft[z] / parRight[z];
		}
		return retval;
	}
} //namespace vwr
