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

#include "vec.hpp"
#include "dummy_functions.hpp"
#include "one_way_conversion.hpp"
#include <cassert>

int main() {
	//just a 2D GLM vector
	gvec2 sample_2d(0.0f);

	//use sizzling to convert to a 3D vector and assign to an Eigen vector
	evec3 eigen_wrapped = sample_2d.xy1();

	//invoke some dummy function that takes an Eigen vector
	print_eigen_vector3(eigen_wrapped.data());

	//let's call the GLM dummy function with the same data now
	const gvec3& glm_ref = eigen_wrapped.cast<gvec3>();
	//you can expect them to have the same address in this case
	assert(reinterpret_cast<intptr_t>(&glm_ref) == reinterpret_cast<intptr_t>(&eigen_wrapped));
	print_glm_vector3(glm_ref.data());

	//more examples here
	one_way_conversion();

	return 0;
}
