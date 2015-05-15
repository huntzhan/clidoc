add_library (clidoc_cpp11_codegen cpp11_codegen.cc)

add_library (clidoc_filesystem filesystem.cc)
target_link_libraries(clidoc_filesystem ${Boost_LIBRARIES})
set_target_properties (
  clidoc_filesystem
  PROPERTIES COMPILE_FLAGS "-DCLIDOC_RESOURCE_DIR=${CMAKE_BINARY_DIR}/resource"
)


# compile library for in-project cpp11 codegen.
set (CLIDOC_CPP11_PREFIX "clidoc_project_used_")
set (PATH_PREFIX "${clidoc_SOURCE_DIR}/resource/cpp11/project_template/src/")

include_directories (${clidoc_SOURCE_DIR}/resource/cpp11/project_template/include/)

add_library (${CLIDOC_CPP11_PREFIX}argv_processor ${PATH_PREFIX}argv_processor.cc)
add_library (${CLIDOC_CPP11_PREFIX}match_logic    ${PATH_PREFIX}match_logic.cc)
add_library (${CLIDOC_CPP11_PREFIX}cpp11          ${PATH_PREFIX}cpp11.cc)

target_link_libraries (
  ${CLIDOC_CPP11_PREFIX}cpp11
  ${CLIDOC_CPP11_PREFIX}argv_processor
  ${CLIDOC_CPP11_PREFIX}match_logic
  clidoc_smart_ptr_interface
)

unset (PATH_PREFIX)
unset (CLIDOC_CPP11_PREFIX)


# compile library for python codegen.
add_library (clidoc_python_codegen python_codegen.cc)
set_target_properties (
  clidoc_python_codegen
  PROPERTIES COMPILE_FLAGS
  "-DPYTHON_CODEGEN_DIR=${CMAKE_BINARY_DIR}/resource/python"
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
    clidoc_project_used_cpp11 ${clidoc_cpp11_lib_name}
  )
  # delete names.
  unset (clidoc_cpp11_lib_name)
  unset (clidoc_cpp11_file_name)
endfunction(clidoc_cpp11_handler)
