# make sure the compiler find the right version of FlexLexer.h
include_directories (${clidoc_SOURCE_DIR}/lib/flex/include)

add_library (generated_scanner_for_doc generated_scanner_for_doc.cc)
add_library (generated_scanner_for_argv generated_scanner_for_argv.cc)
add_library (generated_parser generated_parser.cc)
add_library (tokenizer tokenizer.cc)

set_target_properties(
  generated_scanner_for_doc generated_scanner_for_argv generated_parser
  PROPERTIES
  COMPILE_FLAGS "-w")
