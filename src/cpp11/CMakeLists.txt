
add_library (clidoc_code_gen_logic code_gen_logic.cc)
add_library (clidoc_argv_processor argv_processor.cc)
add_library (clidoc_match_logic    match_logic.cc)
add_library (clidoc_cpp11          cpp11.cc)

target_link_libraries (
  clidoc_argv_processor
  clidoc_string_utils clidoc_tokenizer clidoc_generated_scanner_for_argv
)

target_link_libraries (
  clidoc_cpp11
  clidoc_argv_processor clidoc_match_logic
)
