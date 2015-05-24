add_executable (clidoc_codegen_helper_test codegen_helper_test.cc)
target_link_libraries (
  clidoc_codegen_helper_test
  clidoc_codegen_helper clidoc_smart_ptr_interface
)
add_gtest (clidoc_codegen_helper_test)
