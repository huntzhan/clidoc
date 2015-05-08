add_executable (clidoc_tokenizer_for_argv_test tokenizer_for_argv_test.cc)
target_link_libraries (
  clidoc_tokenizer_for_argv_test
  clidoc_tokenizer clidoc_generated_scanner_for_argv)

add_executable (clidoc_cpp_code_gen_test cpp_code_gen_test.cc)
target_link_libraries (
  clidoc_cpp_code_gen_test
  clidoc_code_gen_logic clidoc_ast_build clidoc_match_logic
)

add_executable (clidoc_argv_processor_test argv_processor_test.cc)
target_link_libraries (clidoc_argv_processor_test clidoc_argv_processor)

add_gtest (clidoc_cpp_code_gen_test)
add_gtest (clidoc_argv_processor_test)
add_gtest (clidoc_tokenizer_for_argv_test)

add_executable (clidoc_example program.cc)
clidoc_cpp11_handler (clidoc_example ${PROJECT_SOURCE_DIR}/test/cpp11_test/test_doc)
