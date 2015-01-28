
#include <vector>
#include <stdexcept>
#include <set>
#include <map>
#include <memory>

#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"
#include "cpp11/match_logic.h"

using std::vector;
using std::set;
using std::string;
using std::map;
using std::shared_ptr;
using std::make_shared;
using std::make_shared;
using std::logic_error;

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

void MatchState::AddBooleanOutcome(
    const Token &key_of_ast, const Token &key_of_argv) {
  boolean_outcome_.at(key_of_ast) = true;
  token_match_state_.at(key_of_argv) = true;
}

void MatchState::AddStringOutcome(
    const Token &key, const Token &value, const bool key_in_tokens) {
  string_outcome_.at(key) = value.value();
  if (key_in_tokens) { token_match_state_.at(key) = true; }
  token_match_state_.at(value) = true;
}

void MatchState::AddStringListOutcome(
    const Token &key, const Token &value, const bool key_in_tokens) {
  string_list_outcome_.at(key).push_back(value.value());
  if (key_in_tokens) { token_match_state_.at(key) = true; }
  token_match_state_.at(value) = true;
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
    : tokens_(tokens), match_state_ptr_(new MatchState) {
  // init `skip_iters_` and `match_state_.token_match_state_`.
  for (auto iter = tokens_.cbegin(); iter != tokens_.cend(); ++iter) {
    const auto &token = *iter;
    skip_iters_[token] = iter;
    match_state_ptr_->token_match_state_[token] = false;
  }

  // init argv match outcome of `match_state_`.
  auto InitBooleanOutcome = [](
      const set<Token> tokens, map<Token, bool> *outcome) {
    for (const auto &token : tokens) {
      (*outcome)[token] = false;
    }
  };
  auto InitStringOutcome = [&](
      const set<Token> tokens, map<Token, string> *outcome) {
    for (const auto &token : tokens) {
      if (info.default_values_.find(token) != info.default_values_.end()) {
        // set default value.
        (*outcome)[token] = info.default_values_.at(token);
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
  InitBooleanOutcome(
      info.unbound_options_,
      &match_state_ptr_->boolean_outcome_);
  InitBooleanOutcome(
      info.commands_,
      &match_state_ptr_->boolean_outcome_);
  InitStringOutcome(
      info.bound_options_,
      &match_state_ptr_->string_outcome_);
  InitStringOutcome(
      info.arguments_,
      &match_state_ptr_->string_outcome_);
  InitStringListOutcome(
      info.oom_bound_options_,
      &match_state_ptr_->string_list_outcome_);
  InitStringListOutcome(
      info.oom_arguments_,
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

Token MatchStateManager::GetFirstUnmatchToken() const {
  for (const auto &token : tokens_) {
    if (match_state_ptr_->token_match_state_.at(token) == false) {
      return token;
    }
  }
  return Token();
}

bool MatchStateManager::MatchKeyValuePair(
    const Token &key,
    bool (MatchState::*checker)(const Token &) const,
    void (MatchState::*mutator)(const Token &, const Token &, const bool)) {
  if (!(match_state_ptr_.get()->*checker)(key)) {
    return false;
  }
  if (key.type() == TerminalType::ARGUMENT) {
    // argument.
    auto token = GetFirstUnmatchToken();
    if (!token.IsEmpty()) {
      (match_state_ptr_.get()->*mutator)(key, token, false);
      return true;
    }
  } else if (skip_iters_.find(key) != skip_iters_.end()) {
    // bound option.
    auto iter = skip_iters_[key];
    ++iter;
    if (iter != tokens_.cend()
        && iter->type() == TerminalType::GENERAL_ELEMENT) {
      (match_state_ptr_.get()->*mutator)(key, *iter, true);
      return true;
    }
  }
  return false;
}

bool MatchStateManager::MatchBooleanKey(const Token &key) {
  if (!match_state_ptr_->IsBooleanKey(key)) {
    return false;
  }
  if (key.type() == TerminalType::COMMAND) {
    auto transformed_key = key;
    transformed_key.set_type(TerminalType::GENERAL_ELEMENT);
    if (skip_iters_.find(transformed_key) != skip_iters_.end()) {
      match_state_ptr_->AddBooleanOutcome(key, transformed_key);
      return true;
    }
  } else if (skip_iters_.find(key) != skip_iters_.end()) {
    match_state_ptr_->AddBooleanOutcome(key, key);
    return true;
  }
  return false;
}

bool MatchStateManager::MatchStringKey(const Token &key) {
  return MatchKeyValuePair(
      key, &MatchState::IsStringKey, &MatchState::AddStringOutcome);
}

bool MatchStateManager::MatchStringListKey(const Token &key) {
  return MatchKeyValuePair(
      key, &MatchState::IsStringListKey, &MatchState::AddStringListOutcome);
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
  for (const auto &map_pair
       : state_manager_.match_state_ptr()->token_match_state_) {
    if (!map_pair.second) {
      return false;
    }
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
