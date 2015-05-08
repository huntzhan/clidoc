# enable include source files of `cpp11`.
include_directories (
  ${clidoc_SOURCE_DIR}/src/cpp11/project_template/include
)
include_directories (${clidoc_SOURCE_DIR}/src/cpp11/project_template)

# build libraries for testing.
set (CLIDOC_CPP11_PREFIX "for_test_")
add_subdirectory (
  ${clidoc_SOURCE_DIR}/src/cpp11/project_template/src
  project_template
)

add_subdirectory (ast_test)
add_subdirectory (cpp11_test)
