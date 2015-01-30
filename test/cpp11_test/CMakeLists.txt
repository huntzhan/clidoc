add_executable (cpp_code_gen_test cpp_code_gen_test.cc)
target_link_libraries (
  cpp_code_gen_test
  cpp_code_gen_logic ast_build match_logic)

add_executable (argv_processor_test argv_processor_test.cc)
target_link_libraries (argv_processor_test argv_processor)

add_gtest (cpp_code_gen_test)
add_gtest (argv_processor_test)

clidoc_cpp11_code_gen (${PROJECT_SOURCE_DIR}/test/cpp11_test/test_doc)
add_executable (example program.cc)
clidoc_cpp11_linker (example)
