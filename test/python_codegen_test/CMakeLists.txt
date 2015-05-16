
add_executable (python_codegen_test python_codegen_test.cc)
target_link_libraries (
  python_codegen_test
  clidoc_smart_ptr_interface clidoc_python_codegen
)
add_gtest (python_codegen_test)
