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

	template <typename V, typename OP, typename CMP, size_type... I>
	class vector_iterator : OP, CMP {
		//static_assert(VectorWrapperInfo<V>::dimensions >= sizeof...(I), "Start index out of valid range");
	public:
		vector_iterator (const V& parFrom, const V& parUpper);
		~vector_iterator() = default;

		vector_iterator& operator++ ();
		const V& operator*() const { return m_current; }
		bool operator!= (const vector_iterator& parOther) const;
		bool operator== (const vector_iterator& parOther) const;

	private:
		V m_current;
		V m_from;
		V m_upper;
	};

	template <typename V, typename OP, typename CMP, size_type... I>
	vector_iterator<V, OP, CMP, I...>::vector_iterator (const V& parFrom, const V& parUpper) :
		m_current(parFrom),
		m_from(parFrom),
		m_upper(parUpper)
	{
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	auto vector_iterator<V, OP, CMP, I...>::operator++ () -> vector_iterator& {
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
	bool vector_iterator<V, OP, CMP, I...>::operator!= (const vector_iterator& parOther) const {
		return m_current != parOther.m_current;
	}

	template <typename V, typename OP, typename CMP, size_type... I>
	bool vector_iterator<V, OP, CMP, I...>::operator== (const vector_iterator& parOther) const {
		return m_current == parOther.m_current;
	}

	template <typename OP, typename CMP, typename T, size_type... I>
	inline vector_iterator<T, OP, CMP, I...> make_vector_iterator (
		const Vec<T>& parFrom,
		const Vec<T>& parUpper,
		bt::number_seq<size_type, I...>
	) {
		return vector_iterator<Vec<T>, OP, CMP, I...>(parFrom, parUpper);
	}

	template <typename OP, typename CMP, typename T, size_type... I>
	inline vector_iterator<T, OP, CMP, I...> make_vector_iterator_end (
		Vec<T> parFrom,
		const Vec<T>& parUpper,
		bt::number_seq<size_type, I...>
	) {
		OP advance_op;
		auto& last = parFrom[sizeof...(I) - 1];
		const auto& upper_last = parUpper[sizeof...(I) - 1];
		last = advance_op(upper_last);
		return vector_iterator<Vec<T>, OP, CMP, I...>(parFrom, parUpper);
	}
} //namespace vwr

#if defined VWR_OUTER_NAMESPACE
} //namespace VWR_OUTER_NAMESPACE
#endif
