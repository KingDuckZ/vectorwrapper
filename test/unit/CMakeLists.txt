project(unit CXX)

add_executable(${PROJECT_NAME}
	${GTEST_MAIN_CPP}
	test_conversions.cpp
	test_ops.cpp
	example.cpp
	test_get_at.cpp
	test_operators.cpp
)

target_link_libraries(${PROJECT_NAME}
	gtest
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE VWR_WITH_IMPLICIT_CONVERSIONS
)

add_test(
	NAME TestNormal
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${PROJECT_NAME}
)
