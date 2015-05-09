add_library (${CLIDOC_CPP11_PREFIX}argv_processor argv_processor.cc)
add_library (${CLIDOC_CPP11_PREFIX}match_logic    match_logic.cc)
add_library (${CLIDOC_CPP11_PREFIX}cpp11          cpp11.cc)

target_link_libraries (
  ${CLIDOC_CPP11_PREFIX}cpp11
  ${CLIDOC_CPP11_PREFIX}argv_processor ${CLIDOC_CPP11_PREFIX}match_logic
)
