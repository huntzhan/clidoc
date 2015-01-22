
add_library (generated_scanner_for_doc  generated_scanner_for_doc.cc)
add_library (generated_scanner_for_argv generated_scanner_for_argv.cc)
add_library (generated_parser           generated_parser.cc)
add_library (parser_proxy               parser_proxy.cc)
add_library (process_logic              process_logic.cc)
add_library (option_record              option_record.cc)
add_library (ast_build                  ast_build.cc)
add_library (smart_ptr_interface        smart_ptr_interface.cc)
add_library (tokenizer                  tokenizer.cc)

# ignore all warning, might exists a better way to do this.
set_target_properties(
  generated_scanner_for_doc generated_scanner_for_argv generated_parser
  PROPERTIES
  COMPILE_FLAGS "-w")

target_link_libraries (
  generated_parser
  smart_ptr_interface generated_scanner_for_doc)
target_link_libraries (process_logic option_record)
target_link_libraries (parser_proxy generated_parser)
target_link_libraries (ast_build parser_proxy process_logic)
