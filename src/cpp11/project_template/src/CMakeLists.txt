add_library (${CLIDOC_CPP11_PREFIX}generated_scanner_for_argv generated_scanner_for_argv.cc)
add_library (${CLIDOC_CPP11_PREFIX}tokenizer                  tokenizer.cc)

add_library (${CLIDOC_CPP11_PREFIX}argv_processor argv_processor.cc)
add_library (${CLIDOC_CPP11_PREFIX}match_logic    match_logic.cc)
add_library (${CLIDOC_CPP11_PREFIX}cpp11          cpp11.cc)


# ignore all warning, might exists a better way to do this.
set_target_properties (
  ${CLIDOC_CPP11_PREFIX}generated_scanner_for_argv
  PROPERTIES
  COMPILE_FLAGS "-w"
)

target_link_libraries (
  ${CLIDOC_CPP11_PREFIX}argv_processor
  ${CLIDOC_CPP11_PREFIX}tokenizer ${CLIDOC_CPP11_PREFIX}generated_scanner_for_argv
)

target_link_libraries (
  ${CLIDOC_CPP11_PREFIX}cpp11
  ${CLIDOC_CPP11_PREFIX}argv_processor ${CLIDOC_CPP11_PREFIX}match_logic
)
