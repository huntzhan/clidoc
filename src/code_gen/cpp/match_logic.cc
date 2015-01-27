
#include <vector>
#include <set>
#include <map>
#include <memory>

#include "code_gen/cpp/match_logic.h"

using std::vector;
using std::set;
using std::string;
using std::map;
using std::shared_ptr;
using std::make_shared;

namespace clidoc {

void SimpleMemento::BackupMatchStateManager(
    const MatchStateManager *match_state_manager_ptr) {
  match_state_ = match_state_manager_ptr->match_state_;
}

void SimpleMemento::RestoreMatchStateManager(
    MatchStateManager *match_state_manager_ptr) {
  match_state_manager_ptr->match_state_ = match_state_;
}

MatchStateManager::MatchStateManager(
    const CppCodeGenInfo &info,
    const vector<Token> &tokens)
    : tokens_(tokens) {
  // init `skip_iters_` and `match_state_.token_match_state_`.
  for (auto iter = tokens_.cbegin(); iter != tokens_.cend(); ++iter) {
    const auto &token = *iter;
    skip_iters_[token] = iter;
    match_state_.token_match_state_[token] = false;
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
      const set<Token> tokens, map<Token, std::vector<std::string>> *outcome) {
    for (const auto &token : tokens) {
      (*outcome)[token] = {};
    }
  };
  InitBooleanOutcome(
      info.unbound_options_,
      &match_state_.boolean_outcome_);
  InitBooleanOutcome(
      info.commands_,
      &match_state_.boolean_outcome_);
  InitStringOutcome(
      info.bound_options_,
      &match_state_.string_outcome_);
  InitStringOutcome(
      info.arguments_,
      &match_state_.string_outcome_);
  InitStringListOutcome(
      info.oom_bound_options_,
      &match_state_.string_list_outcome_);
  InitStringListOutcome(
      info.oom_arguments_,
      &match_state_.string_list_outcome_);
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

}  // namespace clidoc
