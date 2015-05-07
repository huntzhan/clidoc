cmake_minimum_required (VERSION 2.8)
project (clidoc-cpp11 CXX)


# settings related to C++ compilter#.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")

include_directories (include)
include_directories (src)

# here we go.
add_subdirectory (src)
