project(unit CXX)

add_executable(${PROJECT_NAME}
	${GTEST_MAIN_CPP}
	test_conversions.cpp
	test_ops.cpp
	example.cpp
	test_get_at.cpp
	test_operators.cpp
	test_sequence_range.cpp
)

target_link_libraries(${PROJECT_NAME}
	gtest
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE VWR_WITH_IMPLICIT_CONVERSIONS
	PRIVATE VWR_EXTRA_ACCESSORS
)

add_test(
	NAME TestNormal
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${PROJECT_NAME}
)
