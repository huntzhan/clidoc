#ifndef SRC_CPP11_INFO_H_
#define SRC_CPP11_INFO_H_

#include <string>
#include <set>
#include <map>

// TODO(huntzhan): remove dependencies of ast.
#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"

namespace clidoc {

struct CppCodeGenInfo {
  Doc::SharedPtr doc_node_;
  std::set<Token> bound_options_;
  std::set<Token> unbound_options_;
  std::set<Token> arguments_;
  std::set<Token> oom_bound_options_;
  std::set<Token> oom_arguments_;
  std::set<Token> commands_;
  std::map<Token, std::string> default_values_;
  std::map<Token, Token> option_to_representative_option_;
  std::string doc_text_;
};

extern CppCodeGenInfo cpp_code_gen_info;

}  // namespace clidoc
#endif  // SRC_CPP11_INFO_H_
