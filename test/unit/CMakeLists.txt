project(unit CXX)

add_executable(${PROJECT_NAME}
	${GTEST_MAIN_CPP}
	test_conversions.cpp
)

target_link_libraries(${PROJECT_NAME}
	gtest
)
