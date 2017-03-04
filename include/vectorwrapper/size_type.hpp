/*
 * Copyright 2015-2016 Michele "King_DuckZ" Santullo
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

#ifndef id85B9A9D5E08C4C97B0E3E69757C6AA54
#define id85B9A9D5E08C4C97B0E3E69757C6AA54

#include <cstddef>
#if defined(VWR_SIZE_TYPE)
#	include <cstdint>
#endif

#if defined VWR_OUTER_NAMESPACE
namespace VWR_OUTER_NAMESPACE {
#endif

namespace vwr {
#if !defined(VWR_SIZE_TYPE)
	typedef std::size_t size_type;
#else
	typedef VWR_SIZE_TYPE size_type;
#endif
} //namespace vwr

#if defined VWR_OUTER_NAMESPACE
} //namespace VWR_OUTER_NAMESPACE
#endif

#endif
