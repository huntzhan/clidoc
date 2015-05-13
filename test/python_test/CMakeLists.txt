
add_executable (python_codegen_logic_test codegen_logic_test.cc)
target_link_libraries (
  python_codegen_logic_test
  clidoc_smart_ptr_interface clidoc_python_codegen_logic
)
add_gtest (python_codegen_logic_test)
