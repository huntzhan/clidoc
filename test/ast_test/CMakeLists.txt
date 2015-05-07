add_executable (clidoc_ast_build_test ast_build_test.cc)
target_link_libraries (clidoc_ast_build_test clidoc_ast_build)

add_executable (clidoc_ast_nodes_test ast_nodes_test.cc)
target_link_libraries (clidoc_ast_nodes_test clidoc_smart_ptr_interface)

add_executable (clidoc_parser_proxy_test parser_proxy_test.cc)
target_link_libraries (
  clidoc_parser_proxy_test
  clidoc_parser_proxy clidoc_option_record clidoc_process_logic
)

add_executable (clidoc_process_logic_test process_logic_test.cc)
target_link_libraries (
  clidoc_process_logic_test
  clidoc_smart_ptr_interface clidoc_process_logic clidoc_option_record
)

add_executable (clidoc_smart_ptr_test smart_ptr_test.cc)
target_link_libraries (clidoc_smart_ptr_test clidoc_smart_ptr_interface)

add_executable (clidoc_tokenizer_for_doc_test tokenizer_for_doc_test.cc)
target_link_libraries (
  clidoc_tokenizer_for_doc_test
  clidoc_tokenizer clidoc_generated_scanner_for_doc)

add_gtest (clidoc_ast_build_test)
add_gtest (clidoc_ast_nodes_test)
add_gtest (clidoc_parser_proxy_test)
add_gtest (clidoc_process_logic_test)
add_gtest (clidoc_smart_ptr_test)
add_gtest (clidoc_tokenizer_for_doc_test)
