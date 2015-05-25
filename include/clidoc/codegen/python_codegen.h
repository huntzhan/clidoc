#ifndef INCLUDE_CLIDOC_CODEGEN_PYTHON_CODEGEN_H_
#define INCLUDE_CLIDOC_CODEGEN_PYTHON_CODEGEN_H_

#include "clidoc/ast/ast_build.h"

namespace clidoc {

std::string PythonCodegen(const CodeGenInfo &code_gen_info);

}  // namespace clidoc
#endif  // INCLUDE_CLIDOC_CODEGEN_PYTHON_CODEGEN_H_
