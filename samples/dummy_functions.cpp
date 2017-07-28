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

#include "dummy_functions.hpp"
#include <iostream>

void print_eigen_vector3 (const Eigen::Vector3f& parVec) {
	std::cout << "Eigen::Vector3f = <" <<
		parVec.x() << ',' << parVec.y() << ',' << parVec.z() << ">\n";
}

void print_glm_vector3 (const glm::vec3& parVec) {
	std::cout << "glm::vec3 = <" <<
		parVec.x << ',' << parVec.y << ',' << parVec.z << ">\n";
}
