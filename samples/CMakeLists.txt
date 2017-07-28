cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(samples CXX)
list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

find_package(GLM REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(${PROJECT_NAME}
	main.cpp
	one_way_conversion.cpp
	dummy_functions.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(${PROJECT_NAME}
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE VWR_WITH_IMPLICIT_CONVERSIONS
	PRIVATE VWR_EXTRA_ACCESSORS
)

target_include_directories(${PROJECT_NAME} SYSTEM
	PRIVATE ${GLM_INCLUDE_DIRS}
	PRIVATE ${EIGEN3_INCLUDE_DIR}
)

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_SOURCE_DIR}/../include
)
