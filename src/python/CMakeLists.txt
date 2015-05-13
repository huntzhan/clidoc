file (COPY codegen.py DESTINATION .)

add_library (clidoc_python_codegen_logic codegen_logic.cc)
set_target_properties (
  clidoc_python_codegen_logic
  PROPERTIES COMPILE_FLAGS
  "-DPYTHON_CODEGEN_DIR=${CMAKE_BINARY_DIR}/src/python"
)
