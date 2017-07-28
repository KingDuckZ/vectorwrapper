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

#include "one_way_conversion.hpp"
#include "vectorwrapper/vector_ostream.hpp"
#include "dummy_functions.hpp"
#include "vec.hpp"
#include <vector>

namespace vwr {
	//if you want to go crazy you can do one way inplace casting even if
	//the wrapped type's storage is not on the stack
	template <>
	struct VectorWrapperInfo<std::vector<float>> :
		is_castable_to<Eigen::Vector3f>,
		is_castable_to<glm::vec3>
	{
		enum { dimensions = 3 };
		typedef float scalar_type;
		typedef std::vector<float> vector_type;

		static scalar_type& get_at (size_type parIndex, vector_type& parVector) {
			if (parVector.empty())
				parVector.resize(3);
			return parVector[parIndex];
		}
	};
} //namespace vwr

namespace {
	using vvec3 = vwr::Vec<std::vector<float>>;
} //unnamed namespace

void one_way_conversion() {
	vvec3 heap_vector(10.0f, 11.0f, 12.0f);

	std::cout << "wrapped std::vector<float> = " << heap_vector << '\n';

	//casting to glm and eigen is possible since we explicitly allowed it
	auto& eigen_ref = heap_vector.cast<evec3>();

	//...but the other way around won't compile
	//vvec3& not_working = eigen_ref.cast<vvec3>();

	print_eigen_vector3(eigen_ref.data());
}
