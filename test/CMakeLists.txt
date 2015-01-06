add_executable (parser_proxy_test parser_proxy_test.cc)
target_link_libraries (parser_proxy_test parser_proxy)

add_executable (process_logic_test process_logic_test.cc)
target_link_libraries (process_logic_test smart_ptr_interface)

add_executable (smart_ptr_test smart_ptr_test.cc)
target_link_libraries (smart_ptr_test smart_ptr_interface)

add_executable (ast_nodes_test ast_nodes_test.cc)
target_link_libraries (ast_nodes_test smart_ptr_interface)

add_executable (tokenizer_for_argv_test tokenizer_for_argv_test.cc)
add_executable (tokenizer_for_doc_test tokenizer_for_doc_test.cc)
target_link_libraries (
  tokenizer_for_argv_test
  tokenizer token_proxy generated_scanner_for_argv)
target_link_libraries (
  tokenizer_for_doc_test
  tokenizer token_proxy generated_scanner_for_doc)

add_gtest (parser_proxy_test)
add_gtest (process_logic_test)
add_gtest (smart_ptr_test)
add_gtest (ast_nodes_test)
add_gtest (tokenizer_for_argv_test)
add_gtest (tokenizer_for_doc_test)
