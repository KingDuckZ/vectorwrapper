project(unit_noconv CXX)

set(warning_flags "-Wall -Wextra -Wconversion -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warning_flags}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${warning_flags}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${warning_flags}")

add_executable(${PROJECT_NAME}
	${GTEST_MAIN_CPP}
	test_conversions.cpp
)

target_link_libraries(${PROJECT_NAME}
	gtest
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 14)

add_test(
	NAME TestNoImplicitConversion
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${PROJECT_NAME}
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE VWR_OUTER_NAMESPACE=vwr_outer_ns
)
