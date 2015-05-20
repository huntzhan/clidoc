set (CLIDOC_CPP11_PREFIX "clidoc_project_used")

add_executable (clidoc_cpp11_codegen_test  cpp11_codegen_test.cc)
add_executable (clidoc_argv_processor_test argv_processor_test.cc)
add_executable (clidoc_example             program.cc)

target_link_libraries (
  clidoc_cpp11_codegen_test
  clidoc_cpp11_codegen clidoc_ast_build ${CLIDOC_CPP11_PREFIX}_match_logic
)
target_link_libraries (
  clidoc_argv_processor_test
  ${CLIDOC_CPP11_PREFIX}_argv_processor
)

add_gtest (clidoc_cpp11_codegen_test)
add_gtest (clidoc_argv_processor_test)

clidoc_cpp11_handler (
  clidoc_example
  ${PROJECT_SOURCE_DIR}/test/cpp11_codegen_test/test_doc
)


# test docs.
include_directories (${clidoc_SOURCE_DIR}/resource/cpp11/project_template/include/)

function (compile_codegen_test doc_name)
  add_executable (clidoc_${doc_name}_test ${doc_name}_test.cc)
  clidoc_cpp11_handler (
    clidoc_${doc_name}_test
    ${clidoc_SOURCE_DIR}/test/test_docs/${doc_name}
  )
  add_gtest (clidoc_${doc_name}_test)
endfunction(compile_codegen_test)

compile_codegen_test (simple_option)
compile_codegen_test (option_binding)
compile_codegen_test (command)
compile_codegen_test (logic)
compile_codegen_test (default_value)
compile_codegen_test (argument)
