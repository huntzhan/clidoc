#ifndef SRC_CODE_GEN_CPP_INFO_H_
#define SRC_CODE_GEN_CPP_INFO_H_

#include <string>
#include <set>

#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"

namespace clidoc {

struct CppCodeGenInfo {
  Doc::SharedPtr doc_node_;
  std::set<Token> focused_bound_options_;
  std::set<Token> focused_unbound_options_;
  std::set<Token> focused_arguments_;
  std::set<Token> focused_oom_bound_options_;
  std::set<Token> focused_oom_arguments_;
  std::set<Token> focused_commands_;
  std::string doc_text_;
};

extern CppCodeGenInfo cpp_code_gen_info;

}  // namespace clidoc
#endif  // SRC_CODE_GEN_CPP_INFO_H_
