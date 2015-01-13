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
class CodeGenInfo {
 public:
  void Prepare(const std::string &raw_doc);

  Doc::SharedPtr doc_node_;
  OptionBindingRecorder recorder_;

  std::set<Token> focused_bound_options_;
  std::set<Token> focused_unbound_options_;
  std::set<Token> focused_arguments_;
  // `oom` stands for OneOrMore.
  std::set<Token> focused_oom_bound_options_;
  std::set<Token> focused_oom_arguments_;
  std::set<Token> focused_commands_;

  std::string doc_text_;

 private:
  void PostProcessedAST(
      std::set<Token> *focused_elements_ptr,
      std::set<Token> *oom_elements_ptr);
};

}  // namespace clidoc
#endif  // SRC_AST_AST_BUILD_H_
