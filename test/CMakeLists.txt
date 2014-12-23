add_executable (tokenizer_for_doc tokenizer_for_doc.cc)
target_link_libraries (tokenizer_for_doc tokenizer generated_scanner_for_doc)
add_gtest (tokenizer_for_doc)

add_executable (tokenizer_for_argv tokenizer_for_argv.cc)
target_link_libraries (tokenizer_for_argv tokenizer generated_scanner_for_argv)
add_gtest (tokenizer_for_argv)

add_executable (smart_ptr smart_ptr.cc)
target_link_libraries (smart_ptr smart_ptr_interface)
add_gtest (smart_ptr)

add_executable (test_parser_proxy parser_proxy.cc)
target_link_libraries (test_parser_proxy parser_proxy)
add_gtest (test_parser_proxy)

add_executable (symbol symbol.cc)
target_link_libraries (symbol smart_ptr_interface)
add_gtest (symbol)

add_executable (test_process_logic process_logic.cc)
target_link_libraries (test_process_logic smart_ptr_interface)
add_gtest (test_process_logic)
