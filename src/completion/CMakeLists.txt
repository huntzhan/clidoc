ADD_LIBRARY (clidoc_ast_analyser ast_analyser.cc)
ADD_LIBRARY (clidoc_script_generator script_generator.cc)
ADD_LIBRARY (clidoc_compl_string_util compl_string_util.cc)
ADD_LIBRARY (clidoc_completion completion.cc)

TARGET_LINK_LIBRARIES (
  clidoc_ast_analyser
  clidoc_compl_string_util
)

TARGET_LINK_LIBRARIES (
  clidoc_completion 
  clidoc_ast_analyser clidoc_script_generator)
