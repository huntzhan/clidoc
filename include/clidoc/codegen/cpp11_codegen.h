#ifndef INCLUDE_CLIDOC_CODEGEN_CPP11_CODEGEN_H_
#define INCLUDE_CLIDOC_CODEGEN_CPP11_CODEGEN_H_

#include <string>
#include "clidoc/ast/ast_build.h"

namespace clidoc {

std::string Cpp11Codegen(const CodeGenInfo &code_gen_info);

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_CODEGEN_CPP11_CODEGEN_H_
