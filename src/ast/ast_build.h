#ifndef SRC_AST_AST_BUILD_H_
#define SRC_AST_AST_BUILD_H_

#include <string>
#include <set>

#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"
#include "ast/option_record.h"

namespace clidoc {

// 1. Build AST.
// 2. Collect necessary information for code gen procedure.
struct CodeGenInfo {
  void Prepare(const std::string &raw_doc);
  
  Doc::SharedPtr doc_node;
  OptionBindingRecorder recorder;
  std::set<Token> focused_bound_options;
  std::set<Token> focused_unbound_options;
  std::set<Token> focused_arguments;
  std::string doc_text;
};

}  // namespace clidoc
#endif  // SRC_AST_AST_BUILD_H_
