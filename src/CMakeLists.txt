
add_subdirectory (ast)
add_subdirectory (cpp11)

add_executable (main main.cc)
target_link_libraries (main ast_build cpp_code_gen_logic)

function (clidoc_cpp11_code_gen filepath)
  # get `clidoc_cpp11_code_gen`. 
  add_custom_command (
    OUTPUT clidoc_cpp11_code_gen.cc
    COMMAND main ARGS ${filepath} cpp11)
  add_library (clidoc_cpp11_code_gen clidoc_cpp11_code_gen.cc)
  set_target_properties (clidoc_cpp11_code_gen PROPERTIES LINKER_LANGUAGE CXX)
  target_link_libraries (clidoc_cpp11_code_gen smart_ptr_interface)
endfunction (clidoc_cpp11_code_gen)

function (clidoc_cpp11_linker user_defined_program)
  target_link_libraries (${user_defined_program} cpp11 clidoc_cpp11_code_gen)
endfunction (clidoc_cpp11_linker)
