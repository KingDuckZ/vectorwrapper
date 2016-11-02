cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

include(CTest)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++11")

add_subdirectory(gtest-1.7.0)
set(GTEST_MAIN_CPP "${CMAKE_SOURCE_DIR}/gtest-1.7.0/src/gtest_main.cc")
set(UNITTEST_DATA_DIR "${CMAKE_SOURCE_DIR}/../data")

include_directories(SYSTEM
	gtest-1.7.0/include
)
include_directories(../include)
add_subdirectory(unit)
add_subdirectory(unit_noconv)
