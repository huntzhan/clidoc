cmake_minimum_required (VERSION 2.8)
project (clidoc CXX)


# settings related to C++ compilter#.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -O2 -std=c++11")

get_directory_property (has_parent PARENT_DIRECTORY)
if (has_parent)
  set (
    CLIDOC_CPP11_INCLUDE_PATHS
    "${clidoc_SOURCE_DIR}/include"
    "${clidoc_SOURCE_DIR}/src/cpp11/project_template/include/"
    PARENT_SCOPE
  )
endif()


# setup boost.
find_package(
  Boost REQUIRED
)

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
include_directories (${Boost_INCLUDE_DIRS})
include_directories (${GTEST_INCLUDE_DIRS})
# make sure the compiler find the right version of FlexLexer.h
include_directories (${clidoc_SOURCE_DIR}/lib/flex/include)
# include headers with pattern "clidoc/*.h"
include_directories (include)
# include implementations with pattern "src/*.cc"
include_directories (${clidoc_SOURCE_DIR})


# prepare resource for binary..
file (COPY ${clidoc_SOURCE_DIR}/include         DESTINATION resource)
file (COPY ${clidoc_SOURCE_DIR}/src             DESTINATION resource)
file (COPY ${clidoc_SOURCE_DIR}/resource/cpp11  DESTINATION resource)
file (COPY ${clidoc_SOURCE_DIR}/resource/python DESTINATION resource)


# here we go.
add_subdirectory (src)

option (enable_clidoc_test "turn it off when release." ON)
if (${enable_clidoc_test})
  include_directories (test)
  add_subdirectory (test)
endif()
