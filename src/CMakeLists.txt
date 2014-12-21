add_library (generated_scanner_for_doc generated_scanner_for_doc.cc)
add_library (generated_scanner_for_argv generated_scanner_for_argv.cc)
add_library (generated_parser generated_parser.cc)
add_library (tokenizer tokenizer.cc)
add_library (smart_ptr_interface smart_ptr_interface.cc)
add_library (parser_proxy parser_proxy.cc)

set_target_properties(
  generated_scanner_for_doc generated_scanner_for_argv
  PROPERTIES
  COMPILE_FLAGS "-w")

target_link_libraries (generated_parser
  smart_ptr_interface tokenizer generated_scanner_for_doc)
target_link_libraries (parser_proxy generated_parser)
