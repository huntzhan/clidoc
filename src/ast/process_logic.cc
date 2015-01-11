#include <algorithm>
#include <string>
#include <iterator>
#include <set>

#include "ast/ast_node_interface.h"
#include "ast/option_record.h"
#include "ast/process_logic.h"

using std::set;
using std::next;
using std::string;

namespace clidoc {

void StructureOptimizer::ProcessNode(
    LogicAnd::SharedPtr node) {
  RemoveDuplicatedNodes(node);
}

void StructureOptimizer::ProcessNode(
    LogicXor::SharedPtr node) {
  RemoveDuplicatedNodes(node);
  // transfrom to `LogicAnd` when there's only one child.
  if (node->GetSizeOfChildren() == 1) {
    NodeTypeModifier<LogicAnd>::ChangeNonTerminalType(node);
  }
}

void StructureOptimizer::ProcessNode(
    LogicOr::SharedPtr node) {
  RemoveDuplicatedNodes(node);
  // transfrom to `LogicAnd` when there's only one child.
  if (node->GetSizeOfChildren() == 1) {
    NodeTypeModifier<LogicAnd>::ChangeNonTerminalType(node);
  }
}

void StructureOptimizer::ProcessNode(
    LogicOptional::SharedPtr node) {
  RemoveDuplicatedNodes(node);
}

void StructureOptimizer::ProcessNode(
    LogicOneOrMore::SharedPtr node) {
  RemoveDuplicatedNodes(node);
}

AmbiguityHandler::AmbiguityHandler(OptionBindingRecorder *recorder_ptr)
      : recorder_ptr_(recorder_ptr) {/* empty */}

void AmbiguityHandler::ProcessNode(
    GroupedOptions::SharedPtr grouped_options_node) {
  // loop over every character of `GROUPED_OPTIONS` and treat each character as
  // `POSIX_OPTION`, say `option`. If `option` is not recorded yet(not appeared
  // in `Options Section`), record it; If `option` is recorded, there are two
  // situations should be considered:
  //   1. `option` is NOT the last character in `GROUPED_OPTIONS`: If `option`
  //   is bound with an argument(i.e. `-o FILE` in `Options Section`), then the
  //   rest of `GROUPED_OPTIONS` would be treated as the argument of `option`,
  //   and those arguments should be exactly matched; If there's no bounded
  //   argument, then the loop would be moved forward.
  //   2. `option` is the last character in `GROUPED_OPTIONS`: nothing happend.

  // `-hso` -> LogicOr(`-h`, `-s`, `-o`).
  auto logic_or = LogicOr::Init();
  string value = grouped_options_node->token_.value();

  for (auto iter = value.cbegin() + 1;  // ignore prefix `-`.
       iter != value.cend(); ++iter) {
    // build `option` and `remain`.
    auto option = Token(TerminalType::POSIX_OPTION,
                        "-" + string(iter, iter + 1));
    auto remain = Token(TerminalType::ARGUMENT,
                        string(iter + 1, value.cend()));
    // add `option`.
    logic_or->AddChild(PosixOption::Init(option));

    if (!recorder_ptr_->IsRecorded(option)) {
      // `option` not recorded.
      recorder_ptr_->RecordBinding(option, Token());
      continue;
    }
    if (!recorder_ptr_->HasArgument(option)) {
      // `option` not bound with argument.
      continue;
    }
    if (!remain.IsEmpty()) {
      // recording option -> remain binding.
      recorder_ptr_->RecordBinding(option, remain);
      // add `remain`.
      logic_or->AddChild(Argument::Init(remain));
      break;
    }
  }
  // replace original `GroupedOptions`.
  grouped_options_node->node_connection.ReplacedWith(logic_or);
  // process bindings.
  recorder_ptr_->ProcessCachedBindings();
}

void DoubleHyphenHandler::ProcessNode(
    KDoubleHyphen::SharedPtr double_hyphen_node) {
  // change the type of all elements after `--` to `OPERAND`.
  TerminalTypeModifier<Argument> type_modifier;
  auto &conn = double_hyphen_node->node_connection;
  for (auto iter = next(conn.this_iter_);
       iter != conn.children_of_parent_ptr_->end(); ++iter) {
    (*iter)->Accept(&type_modifier);
  }
  // remove `--`.
  conn.children_of_parent_ptr_->erase(conn.this_iter_);
}

FocusedElementCollector::FocusedElementCollector(
    OptionBindingRecorder *recorder_ptr)
    : recorder_ptr_(recorder_ptr) {/* empty */}

set<Token> FocusedElementCollector::GetFocusedElement() {
  set<Token> focused_elements;
  set<Token> bound_arguments;
  const auto &rep_to_property =
      recorder_ptr_->representative_option_to_property_;
  // include all representative options.
  for (auto iter = rep_to_property.cbegin();
       iter != rep_to_property.cend(); ++iter) {
    focused_elements.insert(iter->first);
    if (!iter->second.IsEmpty()) {
      bound_arguments.insert(iter->second.option_argument_);
    }
  }
  // include unbound arguments.
  for (const auto &argument : operand_candidates_) {
    if (bound_arguments.find(argument) == bound_arguments.end()) {
      focused_elements.insert(argument);
    }
  }
  return focused_elements;
}

void FocusedElementCollector::ProcessNode(PosixOption::SharedPtr node) {
  if (!recorder_ptr_->IsRecorded(node->token_)) {
    recorder_ptr_->RecordBinding(node->token_, Token());
  }
}

void FocusedElementCollector::ProcessNode(GnuOption::SharedPtr node) {
  if (!recorder_ptr_->IsRecorded(node->token_)) {
    recorder_ptr_->RecordBinding(node->token_, Token());
  }
}

void FocusedElementCollector::ProcessNode(Argument::SharedPtr node) {
  operand_candidates_.insert(node->token_);
}

void FocusedElementCollector::ProcessNode(Command::SharedPtr node) {
  operand_candidates_.insert(node->token_);
}

BoundArgumentCleaner::BoundArgumentCleaner(
    const std::set<Token> &bound_arguments)
    : bound_arguments_(bound_arguments) { /* empty */ }

void BoundArgumentCleaner::ProcessNode(Argument::SharedPtr node) {
  auto &conn = node->node_connection;
  if (bound_arguments_.find(node->token_) != bound_arguments_.end()) {
    // is bound argument.
    conn.children_of_parent_ptr_->erase(conn.this_iter_);
  }
}

}  // namespace clidoc
