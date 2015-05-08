cmake_minimum_required (VERSION 2.8)
project (clidoc-cpp11 CXX)


# settings related to C++ compilter#.
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -std=c++11")

# make sure the compiler find the right version of FlexLexer.h
include_directories (${clidoc-cpp11_SOURCE_DIR}/lib/flex/include)
include_directories (include)
include_directories (src)

# prepare libraries.
if (EXISTS "${clidoc-cpp11_SOURCE_DIR}/src/ast")
  add_library (clidoc_smart_ptr_interface src/ast/smart_ptr_interface.cc)
endif()

add_subdirectory (src)

# compile codegen source file.
if (EXISTS "${clidoc-cpp11_SOURCE_DIR}/src/codegen.cc")
  add_library (clidoc_cpp11_codegen src/codegen.cc)
  target_link_libraries (
    clidoc_cpp11_codegen
    clidoc_cpp11 clidoc_smart_ptr_interface
  )
endif()
