# enable include source files of `cpp11`.
include_directories (
  ${clidoc_SOURCE_DIR}/src/cpp11/project_template/include
)
include_directories (${clidoc_SOURCE_DIR}/src/cpp11/project_template)

add_subdirectory (ast_test)
add_subdirectory (cpp11_test)
