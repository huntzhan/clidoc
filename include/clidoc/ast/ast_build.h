#ifndef INCLUDE_CLIDOC_AST_AST_BUILD_H_
#define INCLUDE_CLIDOC_AST_AST_BUILD_H_

#include <string>
#include <set>
#include <map>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/ast/option_record.h"

namespace clidoc {

// 1. Build AST.
// 2. Collect necessary information for code gen procedure.
class CodeGenInfo {
 public:
  void Prepare(const std::string &raw_doc);

  Doc::SharedPtr doc_node_;
  OptionBindingRecorder option_recorder_;
  UnboundArgumentDefaultValueRecorder unbound_argument_recorder_;

  std::set<Token> bound_options_;
  std::set<Token> unbound_options_;
  std::set<Token> arguments_;
  // `oom` stands for OneOrMore.
  std::set<Token> oom_bound_options_;
  std::set<Token> oom_arguments_;
  std::set<Token> commands_;
  // default value of bound options and unbound arguments.
  std::map<Token, std::string> default_values_;

  std::string doc_text_;

 private:
  void PrepareFocusedElements(
      std::set<Token> *elements_ptr,
      std::set<Token> *oom_elements_ptr);
  void PostProcessedAST();
};

}  // namespace clidoc
#endif  // INCLUDE_CLIDOC_AST_AST_BUILD_H_
