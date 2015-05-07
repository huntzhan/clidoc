add_library (clidoc_code_gen_logic code_gen_logic.cc)

# TODO(huntzhan): remove following shit.
include_directories (
  ${clidoc_SOURCE_DIR}/src/cpp11/cmake_cpp11_code_gen_project/include
)
include_directories (${clidoc_SOURCE_DIR}/src/cpp11/cmake_cpp11_code_gen_project/src)
add_subdirectory (${clidoc_SOURCE_DIR}/src/cpp11/cmake_cpp11_code_gen_project/src)
