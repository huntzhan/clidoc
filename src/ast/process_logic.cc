#include <algorithm>
#include <string>
#include <iterator>
#include <set>
#include <map>
#include <utility>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/option_record.h"
#include "clidoc/ast/process_logic.h"
#include "clidoc/ast/ast_visitor_helper.h"

using std::set;
using std::multimap;
using std::make_pair;
using std::next;
using std::string;

namespace clidoc {

AmbiguityHandlerLogic::AmbiguityHandlerLogic(
    OptionBindingRecorder *recorder_ptr)
    : recorder_ptr_(recorder_ptr) {/* empty */}

void AmbiguityHandlerLogic::ProcessNode(
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

    if (!recorder_ptr_->OptionIsRecorded(option)) {
      // `option` not recorded.
      recorder_ptr_->RecordSingleOption(option);
      continue;
    }
    if (!recorder_ptr_->OptionIsBound(option)) {
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

void DoubleHyphenHandlerLogic::ProcessNode(
    KDoubleHyphen::SharedPtr double_hyphen_node) {
  // change the type of all elements after `--` to `OPERAND`.
  TerminalTypeModifierLogic<Argument> callable;
  auto type_modifier = GenerateVisitor<TerminalVisitor>(&callable);
  auto &conn = double_hyphen_node->node_connection;
  for (auto iter = next(conn.this_iter_);
       iter != conn.children_of_parent_ptr_->end(); ++iter) {
    (*iter)->Accept(&type_modifier);
  }
  // remove `--`.
  conn.children_of_parent_ptr_->erase(conn.this_iter_);
}

BoundArgumentCleanerLogic::BoundArgumentCleanerLogic(
    const set<Token> &bound_arguments)
    : bound_arguments_(bound_arguments) { /* empty */ }

void BoundArgumentCleanerLogic::ProcessNode(Argument::SharedPtr node) {
  auto &conn = node->node_connection;
  if (bound_arguments_.find(node->token_) != bound_arguments_.end()) {
    // is bound argument.
    conn.children_of_parent_ptr_->erase(conn.this_iter_);
  }
}

FocusedElementCollectorLogic::FocusedElementCollectorLogic(
    OptionBindingRecorder *recorder_ptr)
    : recorder_ptr_(recorder_ptr) {/* empty */}

set<Token> FocusedElementCollectorLogic::GetFocusedElements() {
  recorder_ptr_->ProcessCachedBindings();
  set<Token> focused_elements = recorder_ptr_->GetRepresentativeOptions();
  set<Token> bound_arguments = recorder_ptr_->GetBoundArguments();
  // include unbound arguments.
  for (const auto &argument : operand_candidates_) {
    if (bound_arguments.find(argument) == bound_arguments.end()) {
      focused_elements.insert(argument);
    }
  }
  return focused_elements;
}

OneOrMoreMarkedElementCollectorLogic::OneOrMoreMarkedElementCollectorLogic(
    OptionBindingRecorder *recorder_ptr)
    : recorder_ptr_(recorder_ptr) {/* empty */}

set<Token> OneOrMoreMarkedElementCollectorLogic::GetOneOrMoreMarkedElements() {
  set<Token> bound_arguments = recorder_ptr_->GetBoundArguments();
  set<Token> oom_elements;
  // build reverse mapping.
  multimap<Token, Token> bound_argument_to_rep_option;
  for (const auto &map_pair
       : recorder_ptr_->representative_option_to_property_) {
    bound_argument_to_rep_option.insert(
        make_pair(map_pair.second.option_argument_, map_pair.first));
  }
  // process recorded elements.
  auto end_iter = bound_argument_to_rep_option.end();
  for (const Token &element : node_recorder_logic_.recorded_elements_) {
    auto range = bound_argument_to_rep_option.equal_range(element);
    if (range.first == end_iter) {
      // `element` is not a bound argument.
      if (element.type() == TerminalType::ARGUMENT) {
        // unbound argument.
        oom_elements.insert(element);
      }
      if (recorder_ptr_->OptionIsBound(element)) {
        // add bound option.
        oom_elements.insert(
            recorder_ptr_->option_to_representative_option_[element]);
      }
    } else {
      // `element` is bound argument.
      for (auto iter = range.first; iter != range.second; ++iter) {
        // add bound option.
        oom_elements.insert(iter->second);
      }
    }
  }
  return oom_elements;
}

void OneOrMoreMarkedElementCollectorLogic::ProcessNode(
    LogicOneOrMore::SharedPtr node) {
  auto visitor = GenerateVisitor<TerminalVisitor>(&node_recorder_logic_);
  node->Accept(&visitor);
}


OneOrMoreNodeInsertLogic::OneOrMoreNodeInsertLogic(
    const set<Token> &focused_oom_bound_options)
    : focused_oom_bound_options_(focused_oom_bound_options) { /* empty */ }

}  // namespace clidoc
