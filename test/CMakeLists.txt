add_executable (run_test test.cc)
target_link_libraries (run_test tokenizer)
add_gtest (run_test)
