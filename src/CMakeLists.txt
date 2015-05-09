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
set_target_properties (
  clidoc_main
  PROPERTIES COMPILE_FLAGS "-DCMAKE_BINARY_DIR=${CMAKE_BINARY_DIR} \
                            -I ${clidoc_SOURCE_DIR}/lib/main_doc/include"
)
target_link_libraries (
  clidoc_main
  clidoc_ast_build clidoc_code_gen_logic
  main_doc_cpp11_codegen
)


function (clidoc_cpp11_handler user_defined_program doc_file_path)
  # set filename and libname.
  get_filename_component (clidoc_cpp11_lib_name ${doc_file_path} NAME)
  set (clidoc_cpp11_file_name ${clidoc_cpp11_lib_name}-cpp11.cc)

  # cpp11 code gen.
  add_custom_command (
    OUTPUT ${clidoc_cpp11_file_name}
    COMMAND clidoc_main ARGS "-m" "cpp11_non_project" ${doc_file_path} ${clidoc_cpp11_file_name}
    DEPENDS ${doc_file_path}
  )

  # create library.
  add_library (${clidoc_cpp11_lib_name} ${clidoc_cpp11_file_name})

  # TODO(hutnzhan): change include.
  set_target_properties (
    ${clidoc_cpp11_lib_name}
    PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_FLAGS "-I ${clidoc_SOURCE_DIR}/src/cpp11/project_template/include/"
  )
  target_link_libraries (
    ${clidoc_cpp11_lib_name}
    clidoc_smart_ptr_interface
  )
  # TODO(hutnzhan): ugly hack, fix it!
  target_link_libraries (
    ${user_defined_program}
    ${CLIDOC_CPP11_PREFIX}cpp11 ${clidoc_cpp11_lib_name}
  )
  # delete names.
  unset (clidoc_cpp11_lib_name)
  unset (clidoc_cpp11_file_name)
endfunction(clidoc_cpp11_handler)
