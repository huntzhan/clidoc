add_library (clidoc_generated_scanner_for_doc  generated_scanner_for_doc.cc)
add_library (clidoc_generated_parser           generated_parser.cc)
add_library (clidoc_parser_proxy               parser_proxy.cc)
add_library (clidoc_process_logic              process_logic.cc)
add_library (clidoc_option_record              option_record.cc)
add_library (clidoc_ast_build                  ast_build.cc)
add_library (clidoc_smart_ptr_interface        smart_ptr_interface.cc)
add_library (clidoc_string_utils               string_utils.cc)

# ignore all warning, might exists a better way to do this.
set_target_properties (
  clidoc_generated_scanner_for_doc
  clidoc_generated_parser
  PROPERTIES
  COMPILE_FLAGS "-w"
)

set_target_properties (
  clidoc_generated_parser
  PROPERTIES
  COMPILE_FLAGS "-I ${clidoc_SOURCE_DIR}/include/clidoc/ast"
)

target_link_libraries (
  clidoc_generated_parser
  clidoc_smart_ptr_interface clidoc_generated_scanner_for_doc
)
target_link_libraries (
  clidoc_process_logic
  clidoc_option_record
)
target_link_libraries (
  clidoc_parser_proxy
  clidoc_generated_parser clidoc_string_utils
)
target_link_libraries (
  clidoc_ast_build
  clidoc_parser_proxy clidoc_process_logic
)
