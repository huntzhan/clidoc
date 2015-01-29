
add_library (cpp_code_gen_logic code_gen_logic.cc)
add_library (argv_processor argv_processor.cc)
add_library (match_logic match_logic.cc)
add_library (cpp11 cpp11.cc)

target_link_libraries (
  argv_processor
  string_utils tokenizer generated_scanner_for_argv)

target_link_libraries (
  cpp11
  argv_processor match_logic)
