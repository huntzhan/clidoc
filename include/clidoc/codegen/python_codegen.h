#ifndef INCLUDE_CLIDOC_CODEGEN_PYTHON_CODEGEN_H_
#define INCLUDE_CLIDOC_CODEGEN_PYTHON_CODEGEN_H_

#include <string>
#include "clidoc/ast/ast_build.h"

namespace clidoc {

std::string PythonCodegen(const CodegenInfo &codegen_info);

}  // namespace clidoc
#endif  // INCLUDE_CLIDOC_CODEGEN_PYTHON_CODEGEN_H_
