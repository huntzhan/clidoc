add_executable (clidoc_ast_build_test         ast_build_test.cc)
add_executable (clidoc_ast_nodes_test         ast_nodes_test.cc)
add_executable (clidoc_parser_proxy_test      parser_proxy_test.cc)
add_executable (clidoc_process_logic_test     process_logic_test.cc)
add_executable (clidoc_smart_ptr_test         smart_ptr_test.cc)
add_executable (clidoc_tokenizer_for_doc_test tokenizer_for_doc_test.cc)

add_library (tokenizer_for_doc tokenizer_for_doc.cc)

target_link_libraries (
  clidoc_ast_build_test
  clidoc_ast_build
)
target_link_libraries (
  clidoc_ast_nodes_test
  clidoc_smart_ptr_interface
)
target_link_libraries (
  clidoc_parser_proxy_test
  clidoc_parser_proxy clidoc_option_record clidoc_process_logic
)
target_link_libraries (
  clidoc_process_logic_test
  clidoc_smart_ptr_interface clidoc_process_logic clidoc_option_record
)
target_link_libraries (
  clidoc_smart_ptr_test
  clidoc_smart_ptr_interface
)
target_link_libraries (
  tokenizer_for_doc
  clidoc_generated_scanner_for_doc
)
target_link_libraries (
  clidoc_tokenizer_for_doc_test
  tokenizer_for_doc
)

add_gtest (clidoc_ast_build_test)
add_gtest (clidoc_ast_nodes_test)
add_gtest (clidoc_parser_proxy_test)
add_gtest (clidoc_process_logic_test)
add_gtest (clidoc_smart_ptr_test)
add_gtest (clidoc_tokenizer_for_doc_test)
