cmake_minimum_required (VERSION 2.8)
project (clidoc-cpp11 CXX)

# settings related to C++ compilter#.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")

# define prefix of target names, default to "clidoc_".
if (NOT DEFINED CLIDOC_CPP11_PREFIX)
  set (CLIDOC_CPP11_PREFIX "clidoc_")
endif()

# include headers with pattern "clidoc/*.h"
include_directories (include)
# include implementations with pattern "src/*.cc"
include_directories (${clidoc-cpp11_SOURCE_DIR})

# compile invariant part of the project.
add_library (${CLIDOC_CPP11_PREFIX}argv_processor src/argv_processor.cc)
add_library (${CLIDOC_CPP11_PREFIX}match_logic    src/match_logic.cc)
add_library (${CLIDOC_CPP11_PREFIX}cpp11          src/cpp11.cc)

target_link_libraries (
  ${CLIDOC_CPP11_PREFIX}cpp11
  ${CLIDOC_CPP11_PREFIX}argv_processor ${CLIDOC_CPP11_PREFIX}match_logic
)

# compile codegen related file.
set (
  SMART_PTR_INTERFACE_CC_PATH
  "${clidoc-cpp11_SOURCE_DIR}/src/smart_ptr_interface.cc"
)
set (
  CODEGEN_CC_PATH
  "${clidoc-cpp11_SOURCE_DIR}/src/codegen.cc"
)

if (EXISTS ${CODEGEN_CC_PATH} AND EXISTS ${SMART_PTR_INTERFACE_CC_PATH})
  add_library (${CLIDOC_CPP11_PREFIX}smart_ptr_interface ${SMART_PTR_INTERFACE_CC_PATH})
  add_library (${CLIDOC_CPP11_PREFIX}codegen             ${CODEGEN_CC_PATH})
  target_link_libraries (
    ${CLIDOC_CPP11_PREFIX}cpp11
    ${CLIDOC_CPP11_PREFIX}codegen ${CLIDOC_CPP11_PREFIX}smart_ptr_interface
  )
else()
  message("File not found!")
endif()
