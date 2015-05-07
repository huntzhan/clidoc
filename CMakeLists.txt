
cmake_minimum_required (VERSION 2.8)
project (clidoc CXX)


# settings related to C++ compilter#.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O2 -std=c++11")


# setup googletest
add_subdirectory (lib/gtest-1.7.0)
set (GTEST_LIBRARY gtest)
set (GTEST_MAIN_LIBRARY gtest_main)
set (GTEST_ROOT ${gtest_SOURCE_DIR})

enable_testing ()
find_package (GTest REQUIRED)

# function that create test cases.
function (add_gtest item)
  target_link_libraries (${item} ${GTEST_BOTH_LIBRARIES})
  add_test (${item} ${item})
endfunction (add_gtest)


# set include paths.
include_directories (${GTEST_INCLUDE_DIRS})
# make sure the compiler find the right version of FlexLexer.h
include_directories (${clidoc_SOURCE_DIR}/lib/flex/include)
include_directories (${clidoc_SOURCE_DIR}/lib/boost_1_57_0/include)
include_directories (src)


# here we go.
add_subdirectory (src)

option (enable_clidoc_test "turn it off when release." ON)
if (${enable_clidoc_test})
  include_directories (test)
  add_subdirectory (test)
endif()
