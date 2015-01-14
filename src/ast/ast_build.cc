
#include <set>
#include <string>

#include "ast/ast_build.h"
#include "ast/parser_proxy.h"
#include "ast/ast_node_interface.h"
#include "ast/process_logic.h"
#include "ast/option_record.h"
#include "ast/ast_visitor_helper.h"

using std::set;
using std::string;

namespace clidoc {

void CodeGenInfo::PostProcessedAST(
    set<Token> *focused_elements_ptr,
    set<Token> *oom_elements_ptr) {
  // 1. remove duplicated nodes.
  StructureOptimizerLogic structure_optimizer_logic;
  auto structure_optimizer = GenerateVisitor<NonTerminalVisitor>(
      &structure_optimizer_logic);
  doc_node_->Accept(&structure_optimizer);
  // 2. process `--`.
  DoubleHyphenHandler double_dash_handler;
  doc_node_->Accept(&double_dash_handler);
  // 3. handle ambiguous syntax.
  AmbiguityHandler ambiguity_handler(&recorder_);
  doc_node_->Accept(&ambiguity_handler);
  // 4. collect focused elements.
  FocusedElementCollector focused_element_collector(&recorder_);
  doc_node_->Accept(&focused_element_collector);
  *focused_elements_ptr = focused_element_collector.GetFocusedElements();
  *oom_elements_ptr = focused_element_collector.GetOneOrMoreMarkedElements();
  // 5.TODO(huntzhan): issue #17.
  // 6. Remove bound arguments.
  auto bound_arguments = recorder_.GetBoundArguments();
  BoundArgumentCleaner bound_argument_cleaner(bound_arguments);
  doc_node_->Accept(&bound_argument_cleaner);
  // 7. remove duplicated nodes again.
  doc_node_->Accept(&structure_optimizer);
}

void CodeGenInfo::Prepare(const std::string &raw_doc) {
  set<Token> focused_elements;
  set<Token> oom_elements;
  DocPreprocessor doc_preprocessor;
  ParserProxy parser_proxy;
  // setup `doc_node_`, `recorder_`.
  parser_proxy.Parse(raw_doc, &doc_node_, &recorder_);
  PostProcessedAST(&focused_elements, &oom_elements);
  // setup `doc_text_`.
  doc_text_ = doc_preprocessor.PreprocessRawDocForCodeGen(raw_doc);
  // setup focused elements/
  for (const Token &element : focused_elements) {
    if (recorder_.OptionIsBound(element)) {
      focused_bound_options_.insert(element);
      continue;
    }
    switch (element.type()) {
      case TerminalType::ARGUMENT:
        focused_arguments_.insert(element);
        break;
      case TerminalType::COMMAND:
        focused_commands_.insert(element);
        break;
      default:
        focused_unbound_options_.insert(element);
    }
  }
  // setup `focused_oom_*`.
  for (const Token &element : oom_elements) {
    switch (element.type()) {
      case TerminalType::ARGUMENT:
        focused_oom_arguments_.insert(element);
        break;
      default:
        focused_oom_bound_options_.insert(element);
    }
  }
}

}  // namespace clidoc
