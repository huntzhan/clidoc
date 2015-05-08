cmake_minimum_required (VERSION 2.8)
project (clidoc-cpp11 CXX)

# define prefix of target names, default to "clidoc_".
if (NOT DEFINED CLIDOC_CPP11_PREFIX)
  set (CLIDOC_CPP11_PREFIX "clidoc_")
endif()


# settings related to C++ compilter#.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")

# make sure the compiler find the right version of FlexLexer.h
include_directories (${clidoc-cpp11_SOURCE_DIR}/lib/flex/include)
# include headers with pattern "clidoc/*.h"
include_directories (include)
# include implementations with pattern "src/*.cc"
include_directories (${clidoc-cpp11_SOURCE_DIR})

# prepare libraries.
set (
  SMART_PTR_INTERFACE_CC_PATH
  "${clidoc-cpp11_SOURCE_DIR}/src/smart_ptr_interface.cc"
)
if (EXISTS ${SMART_PTR_INTERFACE_CC_PATH})
  add_library (${CLIDOC_CPP11_PREFIX}smart_ptr_interface ${SMART_PTR_INTERFACE_CC_PATH})
endif()

# compile libraries.
add_subdirectory (src)

# compile codegen source file.
if (EXISTS "${clidoc-cpp11_SOURCE_DIR}/src/codegen.cc")
  add_library (${CLIDOC_CPP11_PREFIX}cpp11_codegen src/codegen.cc)
  target_link_libraries (
    ${CLIDOC_CPP11_PREFIX}cpp11_codegen
    ${CLIDOC_CPP11_PREFIX}cpp11 ${CLIDOC_CPP11_PREFIX}smart_ptr_interface
  )
endif()
