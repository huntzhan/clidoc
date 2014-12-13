# make sure the compiler find the right version of FlexLexer.h
include_directories (${clidoc_SOURCE_DIR}/lib/flex/include)

add_library (generated_scanner generated_scanner.cc)
add_library (generated_parser generated_scanner.cc)
add_library (tokenizer tokenizer.cc)
target_link_libraries (tokenizer generated_scanner)

set_target_properties(
  generated_scanner generated_parser PROPERTIES
  COMPILE_FLAGS "-w")
