
add_library (generated_scanner_for_doc  generated_scanner_for_doc.cc)
add_library (generated_scanner_for_argv generated_scanner_for_argv.cc)
add_library (generated_parser           generated_parser.cc)
add_library (parser_proxy               parser_proxy.cc)
add_library (token_proxy                token_proxy.cc)
add_library (process_logic              process_logic.cc)

# ignore all warning, might exists a better way to do this.
set_target_properties(
  generated_scanner_for_doc generated_scanner_for_argv
  PROPERTIES
  COMPILE_FLAGS "-w")

target_link_libraries (
  generated_parser
  smart_ptr_interface token_proxy generated_scanner_for_doc)
target_link_libraries (
  parser_proxy
  generated_parser)
