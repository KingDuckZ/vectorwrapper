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

#ifndef idD03A8672B1F74408AC6F3B11409CB786
#define idD03A8672B1F74408AC6F3B11409CB786

#include "vectorwrapper/vectorwrapper.hpp"
#include <Eigen/Core>
#include <glm/glm.hpp>

namespace vwr {
	//Setup wrappers for Eigen vectors
	template <>
	struct VectorWrapperInfo<Eigen::Vector2f> :
		is_castable_to<glm::vec2>
	{
		enum { dimensions = 2 };

		typedef float scalar_type;
		typedef Eigen::Vector3f higher_vector_type;
		typedef Eigen::Vector2f vector_type;

		static scalar_type& get_at (size_type parIndex, vector_type& parVector) {
			return parVector.coeffRef(parIndex);
		}
	};
	template <>
	struct VectorWrapperInfo<Eigen::Vector3f> :
		is_castable_to<glm::vec3>
	{
		enum { dimensions = 3 };

		typedef float scalar_type;
		typedef Eigen::Vector2f lower_vector_type;
		typedef Eigen::Vector3f vector_type;

		static scalar_type& get_at (size_type parIndex, vector_type& parVector) {
			return parVector.coeffRef(parIndex);
		}
	};

	//Setup wrappers for GLM vectors
	template <>
	struct VectorWrapperInfo<glm::vec2> {
		enum { dimensions = 2 };

		typedef float scalar_type;
		typedef glm::vec3 higher_vector_type;
		typedef glm::vec2 vector_type;

		enum {
			offset_x = offsetof(glm::vec3, x),
			offset_y = offsetof(glm::vec3, y)
		};
	};

	template <>
	struct VectorWrapperInfo<glm::vec3> {
		enum { dimensions = 3 };

		typedef float scalar_type;
		typedef glm::vec2 lower_vector_type;
		typedef glm::vec3 vector_type;

		enum {
			offset_x = offsetof(glm::vec3, x),
			offset_y = offsetof(glm::vec3, y),
			offset_z = offsetof(glm::vec3, z)
		};
	};
} //namespace vwr

typedef vwr::Vec<Eigen::Vector2f> evec2;
typedef vwr::Vec<Eigen::Vector3f> evec3;
typedef vwr::Vec<glm::vec2> gvec2;
typedef vwr::Vec<glm::vec3> gvec3;

#endif
