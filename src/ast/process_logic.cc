#include <algorithm>
#include <string>
#include <iterator>

#include "ast/ast_node_interface.h"
#include "ast/token_proxy.h"
#include "ast/process_logic.h"

using std::next;
using std::string;

namespace clidoc {

void StructureOptimizer::ProcessNode(
    Doc::SharedPtr node_ptr) {
  RemoveDuplicatedNodes(node_ptr);
}

void StructureOptimizer::ProcessNode(
    LogicAnd::SharedPtr node_ptr) {
  RemoveDuplicatedNodes(node_ptr);
}

void StructureOptimizer::ProcessNode(
    LogicXor::SharedPtr node_ptr) {
  RemoveDuplicatedNodes(node_ptr);
}

AmbiguityHandler::AmbiguityHandler(OptionBindingRecorder *recorder_ptr)
      : recorder_ptr_(recorder_ptr) {/* empty */}

void AmbiguityHandler::ProcessNode(
    GroupedOptions::SharedPtr grouped_options_ptr) {
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

  // `-hso` -> `-h -s -o`.
  auto logic_and = LogicAnd::Init();
  string value = grouped_options_ptr->token_.value();

  for (auto iter = value.cbegin() + 1;  // ignore prefix `-`.
       iter != value.cend(); ++iter) {
    // build `option` and `remain`.
    auto option = InitToken(TerminalType::POSIX_OPTION,
                            "-" + string(iter, iter + 1));
    auto remain = InitToken(TerminalType::ARGUMENT,
                            string(iter + 1, value.cend()));
    // add `option`.
    logic_and->AddChild(PosixOption::Init(option));

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
      logic_and->AddChild(Argument::Init(remain));
      break;
    }
  }
  // replace original `GroupedOptions`.
  *grouped_options_ptr->node_connection.this_iter_ = logic_and;
  logic_and->node_connection.ConnectParent(
      &grouped_options_ptr->node_connection);
  // process bindings.
  recorder_ptr_->ProcessCachedBindings();
}

void DoubleHyphenHandler::ProcessNode(
    KDoubleHyphen::SharedPtr double_hyphen_ptr) {
  // change the type of all elements after `--` to `OPERAND`.
  TerminalTypeModifier<Argument> type_modifier;
  auto &conn = double_hyphen_ptr->node_connection;
  for (auto iter = next(conn.this_iter_);
       iter != conn.children_of_parent_ptr_->end(); ++iter) {
    (*iter)->Accept(&type_modifier);
  }
  // remove `--`.
  conn.children_of_parent_ptr_->erase(conn.this_iter_);
}

// FocusedElementCollector::FocusedElementCollector(
//     OptionBindingRecorder *recorder_ptr)
//     : recorder_ptr_(recorder_ptr) {/* empty */}

}  // namespace clidoc
