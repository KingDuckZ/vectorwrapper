project(speed CXX)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -mfpmath=sse -mmmx -msse -msse2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mfpmath=sse -mmmx -msse -msse2")

add_executable(${PROJECT_NAME}
	${GTEST_MAIN_CPP}
	speed.cpp
)

target_link_libraries(${PROJECT_NAME}
	gtest
)
add_test(
	NAME SpeedTest
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${PROJECT_NAME}
)
