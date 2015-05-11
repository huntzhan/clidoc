add_subdirectory (ast)
add_subdirectory (cpp11)

# for cmake project generation.
file (COPY ${clidoc_SOURCE_DIR}/include DESTINATION files)
file (COPY ${clidoc_SOURCE_DIR}/src DESTINATION files)

set (CLIDOC_CPP11_PREFIX "main_doc_")
add_subdirectory (${clidoc_SOURCE_DIR}/lib/main_doc clidoc_main_doc)
unset (CLIDOC_CPP11_PREFIX)

add_executable (
  clidoc_main main.cc
)
set (
  CLIDOC_MAIN_COMPILE_FLAGS
  "-DCMAKE_BINARY_DIR=${CMAKE_BINARY_DIR} -I ${clidoc_SOURCE_DIR}/lib/main_doc/include"
)
set_target_properties (
  clidoc_main
  PROPERTIES COMPILE_FLAGS ${CLIDOC_MAIN_COMPILE_FLAGS}
)
target_link_libraries (
  clidoc_main
  clidoc_ast_build clidoc_code_gen_logic
  main_doc_cpp11
)
