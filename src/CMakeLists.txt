add_subdirectory (ast)
add_subdirectory (codegen)

set (CLIDOC_CPP11_PREFIX "main_doc_")
add_subdirectory (${clidoc_SOURCE_DIR}/lib/main_doc clidoc_main_doc)
unset (CLIDOC_CPP11_PREFIX)

add_executable (
  clidoc_main main.cc
)
set_target_properties (
  clidoc_main
  PROPERTIES COMPILE_FLAGS "-I ${clidoc_SOURCE_DIR}/lib/main_doc/include"
)
target_link_libraries (
  clidoc_main
  clidoc_ast_build clidoc_cpp11_codegen clidoc_python_codegen
  clidoc_filesystem main_doc_cpp11
)
