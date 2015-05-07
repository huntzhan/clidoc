add_library (clidoc_generated_scanner_for_argv generated_scanner_for_argv.cc)
add_library (clidoc_tokenizer                  tokenizer.cc)

add_library (clidoc_argv_processor argv_processor.cc)
add_library (clidoc_match_logic    match_logic.cc)
add_library (clidoc_cpp11          cpp11.cc)


# ignore all warning, might exists a better way to do this.
set_target_properties (
  clidoc_generated_scanner_for_argv
  PROPERTIES
  COMPILE_FLAGS "-w"
)

target_link_libraries (
  clidoc_argv_processor
  clidoc_tokenizer clidoc_generated_scanner_for_argv
)

target_link_libraries (
  clidoc_cpp11
  clidoc_argv_processor clidoc_match_logic
)
