project(unit CXX)

add_executable(${PROJECT_NAME}
	${GTEST_MAIN_CPP}
	test_conversions.cpp
	test_ops.cpp
	example.cpp
)

target_link_libraries(${PROJECT_NAME}
	gtest
)

add_test(
	NAME UnitTest
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${PROJECT_NAME}
)
