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

#include "vectorwrapper.hpp"
#include "sequence_bt.hpp"
#include "size_type.hpp"
#include <iterator>
#include <ciso646>
#include <cassert>
#include <array>

#if defined VWR_OUTER_NAMESPACE
namespace VWR_OUTER_NAMESPACE {
#endif

namespace vwr {
	namespace op {
		template <typename T, T Inc=1>
		struct inc {
			T operator() (T parVal) const { return parVal + Inc; }
		};

		template <typename T, T Inc=1>
		struct dec {
			T operator() (T parVal) const { return parVal - Inc; }
		};
	} //namespace op

	namespace implem {
		//sets value to the index of the first I >= MAX or to sizeof...(I) if
		//no such value is found
		template <size_type MAX, size_type IDX, size_type... I>
		struct find_ge_than_max;

		template <size_type MAX, size_type IDX, size_type I>
		struct find_ge_than_max<MAX, IDX, I> {
			static constexpr size_type value =
				IDX + (I >= MAX ? 0 : 1);
		};
		template <size_type MAX, size_type IDX, size_type L, size_type... I>
		struct find_ge_than_max<MAX, IDX, L, I...> {
			static constexpr size_type value =
				(L >= MAX ? IDX : find_ge_than_max<MAX, IDX + 1, I...>::value);
		};

		//helper to identify the invalid index in the build error messages
		template <size_type INVALID_INDEX, bool INVALID>
		struct is_invalid_index {
			static_assert(not INVALID,
				"Invalid index specified, indices must all be >= 0 and <= dimensions"
			);
			enum {
				value = 0
			};
		};
	} //namespace implem

	template <typename V, typename OP, typename CMP, size_type... I>
	class sequence_range_iterator : OP, CMP {
		static_assert(not implem::is_invalid_index<
			implem::find_ge_than_max<V::dimensions, 0, I...>::value,
			sizeof...(I) != implem::find_ge_than_max<V::dimensions, 0, I...>::value
		>::value, "");
		static_assert(sizeof...(I) > 0, "At least one index must be specified");
	public:
		sequence_range_iterator (const V& parFrom, const V& parUpper);
		sequence_range_iterator (const V& parCurrent, const V& parFrom, const V& parUpper);
		~sequence_range_iterator() = default;

		sequence_range_iterator& operator++ ();
		const V& operator*() const { return m_current; }
		bool operator!= (const sequence_range_iterator& parOther) const;
		bool operator== (const sequence_range_iterator& parOther) const;

	private:
		V m_current;
		const V& m_from;
		const V& m_upper;
	};

	template <typename V, typename OP, typename CMP, size_type... I>
	class sequence_range {
	public:
		typedef sequence_range_iterator<V, OP, CMP, I...> const_iterator;

		sequence_range (const V& parFrom, const V& parUpper);
		const_iterator cbegin() const;
		const_iterator cend() const;
		const_iterator begin() const { return cbegin(); }
		const_iterator end() const { return cend(); }

	private:
		V m_from;
		V m_upper;
		V m_end;
	};

	namespace implem {
		template <typename T, typename OP, typename CMP, typename U>
		struct make_sequence_helper;

		template <typename T, typename OP, typename CMP, size_type... I>
		struct make_sequence_helper<T, OP, CMP, bt::number_seq<size_type, I...>> {
			typedef sequence_range<Vec<T>, OP, CMP, I...> type;
		};

		template <typename V>
		inline V make_end_vector (V parFrom, const V& parUpper) {
			auto& last = parFrom[V::dimensions - 1];
			const auto& upper_last = parUpper[V::dimensions - 1];
			last = upper_last;
			return parFrom;
		}
	} //namespace implem

	template <typename V, typename OP, typename CMP, size_type... I>
	sequence_range_iterator<V, OP, CMP, I...>::sequence_range_iterator (const V& parFrom, const V& parUpper) :
		m_current(parFrom),
		m_from(parFrom),
		m_upper(parUpper)
	{
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	sequence_range_iterator<V, OP, CMP, I...>::sequence_range_iterator (const V& parCurrent, const V& parFrom, const V& parUpper) :
		m_current(parCurrent),
		m_from(parFrom),
		m_upper(parUpper)
	{
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	auto sequence_range_iterator<V, OP, CMP, I...>::operator++ () -> sequence_range_iterator& {
		static_assert(sizeof...(I) > 0, "At least one index must be given");
		const OP& advance_op = *static_cast<OP*>(this);
		const CMP& cmp_op = *static_cast<CMP*>(this);

		std::array<size_type, sizeof...(I)> lst {I...};
		size_type index = lst[0];
		m_current[index] = advance_op(m_current[index]);
		if (1 < sizeof...(I) and not cmp_op(m_current[index], m_upper[index])) {
			size_type count = 1;
			do {
				m_current[index] = m_from[index];
				index = lst[count++];
				m_current[index] = advance_op(m_current[index]);
			} while (count < sizeof...(I) and not cmp_op(m_current[index], m_upper[index]));
		}

		return *this;
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	bool sequence_range_iterator<V, OP, CMP, I...>::operator!= (const sequence_range_iterator& parOther) const {
		assert(&m_from == &parOther.m_from and &m_upper == &parOther.m_upper);
		return m_current != parOther.m_current;
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	bool sequence_range_iterator<V, OP, CMP, I...>::operator== (const sequence_range_iterator& parOther) const {
		assert(&m_from == &parOther.m_from and &m_upper == &parOther.m_upper);
		return m_current == parOther.m_current;
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	sequence_range<V, OP, CMP, I...>::sequence_range (const V& parFrom, const V& parUpper) :
		m_from(parFrom),
		m_upper(parUpper),
		m_end(implem::make_end_vector<V>(parFrom, parUpper))
	{
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	auto sequence_range<V, OP, CMP, I...>::cbegin() const -> const_iterator {
		return sequence_range_iterator<V, OP, CMP, I...>(m_from, m_upper);
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	auto sequence_range<V, OP, CMP, I...>::cend() const -> const_iterator {
		return sequence_range_iterator<V, OP, CMP, I...>(m_end, m_from, m_upper);
	}

	template <typename V>
	using increasing_sequence_range = typename implem::make_sequence_helper<
		typename V::vector_type,
		op::inc<typename V::scalar_type>,
		std::less<typename V::scalar_type>,
		bt::number_range<size_type, 0, V::dimensions>
	>::type;
} //namespace vwr

#if defined VWR_OUTER_NAMESPACE
} //namespace VWR_OUTER_NAMESPACE
#endif
