set (CLIDOC_CPP11_PREFIX "clidoc_project_used_")

add_executable (clidoc_cpp_code_gen_test cpp_code_gen_test.cc)
target_link_libraries (
  clidoc_cpp_code_gen_test
  clidoc_code_gen_logic clidoc_ast_build ${CLIDOC_CPP11_PREFIX}match_logic
)

add_executable (clidoc_argv_processor_test argv_processor_test.cc)
target_link_libraries (clidoc_argv_processor_test ${CLIDOC_CPP11_PREFIX}argv_processor)

add_gtest (clidoc_cpp_code_gen_test)
add_gtest (clidoc_argv_processor_test)

add_executable (clidoc_example program.cc)
clidoc_cpp11_handler (clidoc_example ${PROJECT_SOURCE_DIR}/test/cpp11_test/test_doc)


# test docs.
include_directories (${clidoc_SOURCE_DIR}/src/cpp11/project_template/include/)

function (compile_codegen_test doc_name)
  add_executable (clidoc_${doc_name}_test test_${doc_name}.cc)
  clidoc_cpp11_handler (
    clidoc_${doc_name}_test
    ${clidoc_SOURCE_DIR}/test/test_docs/${doc_name}
  )
  add_gtest (clidoc_${doc_name}_test)
endfunction(compile_codegen_test)

compile_codegen_test (simple_option)

