
#include <algorithm>
#include <iterator>
#include <vector>
#include <stdexcept>
#include <set>
#include <map>
#include <memory>
#include <utility>

// TODO(huntzhan): remove dependencies of ast.
#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"

#include "match_logic.h"

using std::vector;
using std::set;
using std::string;
using std::map;
using std::shared_ptr;
using std::make_shared;
using std::make_pair;
using std::logic_error;
using std::distance;
using std::all_of;

namespace clidoc {

bool MatchState::IsBooleanKey(const Token &key) const {
  return boolean_outcome_.find(key) != boolean_outcome_.cend();
}

bool MatchState::IsStringKey(const Token &key) const {
  return string_outcome_.find(key) != string_outcome_.cend();
}

bool MatchState::IsStringListKey(const Token &key) const {
  return string_list_outcome_.find(key) != string_list_outcome_.cend();
}

void MatchState::AddBooleanOutcome(const Token &key) {
  boolean_outcome_.at(key) = true;
}

void MatchState::AddStringOutcome(const Token &key, const Token &value) {
  string_outcome_.at(key) = value.value();
}

void MatchState::AddStringListOutcome(const Token &key, const Token &value) {
  string_list_outcome_.at(key).push_back(value.value());
}

void MatchState::MarkArgumentConsumed(const int &index) {
  token_match_state_.at(index) = true;
}

bool MatchState::ArgumentIsConcumed(const int &index) const {
  return token_match_state_.at(index);
}

void SimpleMemento::BackupMatchStateManager(
    const MatchStateManager *match_state_manager_ptr) {
  match_state_ptr_ = make_shared<MatchState>(
      *match_state_manager_ptr->match_state_ptr_);
}

void SimpleMemento::RestoreMatchStateManager(
    MatchStateManager *match_state_manager_ptr) {
  match_state_manager_ptr->match_state_ptr_ = match_state_ptr_;
}

MatchStateManager::MatchStateManager(const CppCodeGenInfo &info,
                                     const vector<Token> &tokens)
    : info_(info), tokens_(tokens), match_state_ptr_(new MatchState) {
  // lambdas.
  auto InitBooleanOutcome = [](
      const set<Token> tokens, map<Token, bool> *outcome) {
    for (const auto &token : tokens) {
      (*outcome)[token] = false;
    }
  };
  auto InitStringOutcome = [&](
      const set<Token> tokens, map<Token, string> *outcome) {
    for (const auto &token : tokens) {
      if (info_.default_values_.find(token) != info_.default_values_.end()) {
        // set default value.
        (*outcome)[token] = info_.default_values_.at(token);
      } else {
        (*outcome)[token] = "";
      }
    }
  };
  auto InitStringListOutcome = [](
      const set<Token> tokens, map<Token, vector<string>> *outcome) {
    for (const auto &token : tokens) {
      (*outcome)[token] = {};
    }
  };

  // init `skip_iters_` and `match_state_.token_match_state_`.
  match_state_ptr_->token_match_state_ = vector<bool>(tokens_.size(), false);
  for (auto iter = tokens_.cbegin(); iter != tokens_.cend(); ++iter) {
    skip_iters_.insert(make_pair(*iter, iter));
  }
  // init argv match outcome of `match_state_`.
  InitBooleanOutcome(
      info_.unbound_options_,
      &match_state_ptr_->boolean_outcome_);
  InitBooleanOutcome(
      info_.commands_,
      &match_state_ptr_->boolean_outcome_);
  InitStringOutcome(
      info_.bound_options_,
      &match_state_ptr_->string_outcome_);
  InitStringOutcome(
      info_.arguments_,
      &match_state_ptr_->string_outcome_);
  InitStringListOutcome(
      info_.oom_bound_options_,
      &match_state_ptr_->string_list_outcome_);
  InitStringListOutcome(
      info_.oom_arguments_,
      &match_state_ptr_->string_list_outcome_);
}

shared_ptr<MementoInterface> MatchStateManager::CreateMemento() const {
  auto memento = make_shared<SimpleMemento>();
  memento->BackupMatchStateManager(this);
  return memento;
}

void MatchStateManager::RestoreFromMemento(
    shared_ptr<MementoInterface> memento) {
  memento->RestoreMatchStateManager(this);
}

void MatchStateManager::PushRollbackPoint() {
  memento_stack_.push(CreateMemento());
}

void MatchStateManager::PopRollbackPoint() {
  memento_stack_.pop();
}

void MatchStateManager::Rollback() {
  RestoreFromMemento(memento_stack_.top());
  PopRollbackPoint();
}

vector<Token>::const_iterator
MatchStateManager::GetFirstUnmatchArgument() const {
  for (auto iter = tokens_.cbegin(); iter != tokens_.cend(); ++iter) {
    auto consumed = match_state_ptr_->ArgumentIsConcumed(
        distance(tokens_.cbegin(), iter));
    if (!consumed) {
      return iter;
    }
  }
  return tokens_.cend();
}

vector<Token>::const_iterator MatchStateManager::GetFirstUnmatchArgument(
    vector<Token>::const_iterator begin_iter) const {
  for (auto iter = begin_iter; iter != tokens_.cend(); ++iter) {
    auto consumed = match_state_ptr_->ArgumentIsConcumed(
        distance(tokens_.cbegin(), iter));
    if (!consumed) {
      return iter;
    }
  }
  return tokens_.cend();
}

vector<Token>::const_iterator MatchStateManager::GetIteratorOfKey(
    const Token &key) const {
  auto range = skip_iters_.equal_range(key);
  if (range.first == range.second) {
    return tokens_.cend();
  }
  auto iter = range.first;
  for (; iter != range.second; ++iter) {
    auto consumed = match_state_ptr_->ArgumentIsConcumed(
        distance(tokens_.cbegin(), iter->second));
    if (!consumed) {
      return iter->second;
    }
  }
  return (--iter)->second;
}

bool MatchStateManager::MatchBooleanKey(const Token &key) {
  if (!match_state_ptr_->IsBooleanKey(key)) {
    return false;
  }
  if (key.type() == TerminalType::COMMAND) {
    auto transformed_key = key;
    transformed_key.set_type(TerminalType::GENERAL_ELEMENT);
    auto key_iter = GetIteratorOfKey(transformed_key);
    if (key_iter != tokens_.cend()) {
      match_state_ptr_->AddBooleanOutcome(key);
      match_state_ptr_->MarkArgumentConsumed(
          distance(tokens_.cbegin(), key_iter));
      return true;
    }
  } else {
    auto key_iter = GetIteratorOfKey(key);
    if (key_iter != tokens_.cend()) {
      match_state_ptr_->AddBooleanOutcome(key);
      match_state_ptr_->MarkArgumentConsumed(
          distance(tokens_.cbegin(), key_iter));
      return true;
    }
  }
  return false;
}

bool MatchStateManager::MatchStringKey(const Token &key) {
  if (!match_state_ptr_->IsStringKey(key)) {
    return false;
  }

  if (key.type() == TerminalType::ARGUMENT) {
    // argument.
    auto value_iter = GetFirstUnmatchArgument();
    if (value_iter != tokens_.cend()) {
      match_state_ptr_->AddStringOutcome(key, *value_iter);
      match_state_ptr_->MarkArgumentConsumed(
          distance(tokens_.cbegin(), value_iter));
      return true;
    }
  } else {
    // bound option.

    auto key_iter = GetIteratorOfKey(key);
    if (key_iter == tokens_.cend()) {
      return false;
    }
    auto value_iter = key_iter + 1;
    // value_iter invalid.
    if (value_iter == tokens_.cend()
        || value_iter->type() != TerminalType::GENERAL_ELEMENT) {
      if (info_.default_values_.find(*key_iter)
          != info_.default_values_.cend()) {
        match_state_ptr_->MarkArgumentConsumed(
            distance(tokens_.cbegin(), key_iter));
        return true;
      } else {
        return false;
      }
    }
    // record key-value pair.
    match_state_ptr_->AddStringOutcome(key, *value_iter);
    // mark key-value pair consumed.
    match_state_ptr_->MarkArgumentConsumed(
        distance(tokens_.cbegin(), key_iter));
    match_state_ptr_->MarkArgumentConsumed(
        distance(tokens_.cbegin(), value_iter));
    return true;
  }
  return false;
}

bool MatchStateManager::MatchStringListKey(const Token &key) {
  if (!match_state_ptr_->IsStringListKey(key)) {
    return false;
  }

  if (key.type() == TerminalType::ARGUMENT) {
    // argument.
    auto value_iter = GetFirstUnmatchArgument();
    if (value_iter != tokens_.cend()) {
      match_state_ptr_->AddStringListOutcome(key, *value_iter);
      match_state_ptr_->MarkArgumentConsumed(
          distance(tokens_.cbegin(), value_iter));
      return true;
    }
  } else {
    // bound option.
    auto key_iter = GetIteratorOfKey(key);
    if (key_iter == tokens_.cend()) {
      return false;
    }
    auto value_iter = GetFirstUnmatchArgument(key_iter + 1);
    if (value_iter != tokens_.cend()
        && value_iter->type() == TerminalType::GENERAL_ELEMENT) {
      match_state_ptr_->AddStringListOutcome(key, *value_iter);
      // mark key-value pair consumed.
      match_state_ptr_->MarkArgumentConsumed(
          distance(tokens_.cbegin(), key_iter));
      match_state_ptr_->MarkArgumentConsumed(
          distance(tokens_.cbegin(), value_iter));
      return true;
    }
  }
  return false;
}

bool MatchStateManager::MatchToken(const Token &key) {
  return MatchBooleanKey(key)
         || MatchStringKey(key)
         || MatchStringListKey(key);
}

shared_ptr<MatchState> MatchStateManager::match_state_ptr() const {
  return match_state_ptr_;
}

MatchStrategy::MatchStrategy(const CppCodeGenInfo &info,
                             const vector<Token> &tokens)
    : state_manager_(info, tokens) { /* empty */ }

bool MatchStrategy::AllMatch() const {
  const auto &flags = state_manager_.match_state_ptr()->token_match_state_;
  if (!all_of(flags.cbegin(), flags.cend(), [](bool c){ return c; })) {
    return false;
  }
  return child_match_condition_;
}

shared_ptr<MatchState> MatchStrategy::GetMatchStatePtr() const {
  return state_manager_.match_state_ptr();
}

void MatchStrategy::ProcessNode(LogicAnd::SharedPtr node) {
  state_manager_.PushRollbackPoint();
  for (auto child_node : node->children_) {
    child_node->Accept(this);
    if (!child_match_condition_) {
      state_manager_.Rollback();
      child_match_condition_ = false;
      return;
    }
  }
  child_match_condition_ = true;
  state_manager_.PopRollbackPoint();
}

void MatchStrategy::ProcessNode(LogicXor::SharedPtr node) {
  for (auto child_node : node->children_) {
    child_node->Accept(this);
    if (child_match_condition_) {
      child_match_condition_ = true;
      return;
    }
  }
  child_match_condition_ = false;
}

void MatchStrategy::ProcessNode(LogicOr::SharedPtr node) {
  bool one_or_more = false;
  for (auto child_node : node->children_) {
    child_node->Accept(this);
    if (child_match_condition_) {
      one_or_more = true;
    }
  }
  child_match_condition_ = one_or_more;
}

void MatchStrategy::ProcessNode(LogicOptional::SharedPtr node) {
  node->children_.front()->Accept(this);
  child_match_condition_ = true;
}

void MatchStrategy::ProcessNode(LogicOneOrMore::SharedPtr node) {
  auto child_node = node->children_.front();
  int counter = -1;
  do {
    child_node->Accept(this);
    ++counter;
  } while (child_match_condition_);
  child_match_condition_ = counter > 0 ? true : false;
}

void MatchStrategy::ProcessNode(PosixOption::SharedPtr node) {
  child_match_condition_ = state_manager_.MatchToken(node->token_);
}

void MatchStrategy::ProcessNode(GnuOption::SharedPtr node) {
  child_match_condition_ = state_manager_.MatchToken(node->token_);
}

void MatchStrategy::ProcessNode(Command::SharedPtr node) {
  child_match_condition_ = state_manager_.MatchToken(node->token_);
}

void MatchStrategy::ProcessNode(Argument::SharedPtr node) {
  child_match_condition_ = state_manager_.MatchToken(node->token_);
}

void MatchStrategy::ProcessNode(GroupedOptions::SharedPtr node) {
  throw logic_error("MatchStrategy::ProcessNode("
                    "GroupedOptions::SharedPtr node)");
}

}  // namespace clidoc
