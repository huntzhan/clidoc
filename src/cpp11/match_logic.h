#ifndef SRC_CPP11_MATCH_LOGIC_H_
#define SRC_CPP11_MATCH_LOGIC_H_

#include <stack>
#include <memory>
#include <map>
#include <string>
#include <vector>

#include "ast/ast_node_interface.h"
#include "ast/ast_nodes.h"
#include "cpp11/info.h"

namespace clidoc {

struct MatchState {
  // type checker.
  bool IsBooleanKey(const Token &key) const;
  bool IsStringKey(const Token &key) const;
  bool IsStringListKey(const Token &key) const;

  void AddBooleanOutcome(const Token &key);
  void AddStringOutcome(const Token &key, const Token &value);
  void AddStringListOutcome(const Token &key, const Token &value);

  void MarkArgumentConsumed(const int &index);
  bool ArgumentIsConcumed(const int &index) const;

  // input arguments.
  std::vector<bool> token_match_state_;
  // argv match outcome.
  std::map<Token, bool> boolean_outcome_;
  std::map<Token, std::string> string_outcome_;
  std::map<Token, std::vector<std::string>> string_list_outcome_;
};

class MatchStateManager;

struct MementoInterface {
  virtual void BackupMatchStateManager(
      const MatchStateManager *match_state_manager_ptr) = 0;
  virtual void RestoreMatchStateManager(
      MatchStateManager *match_state_manager_ptr) = 0;
};

class SimpleMemento : public MementoInterface {
 public:
  void BackupMatchStateManager(
      const MatchStateManager *match_state_manager_ptr) override;
  void RestoreMatchStateManager(
      MatchStateManager *match_state_manager_ptr) override;

 private:
  std::shared_ptr<MatchState> match_state_ptr_;
};

class MatchStateManager {
 public:
  MatchStateManager(const CppCodeGenInfo &info,
                    const std::vector<Token> &tokens);
  // memento pattern related.
  void PushRollbackPoint();
  void PopRollbackPoint();
  void Rollback();
  // modify argv match outcome.
  bool MatchToken(const Token &key);
  std::shared_ptr<MatchState> match_state_ptr() const;

 private:
  // memento pattern related.
  friend class SimpleMemento;
  std::shared_ptr<MementoInterface> CreateMemento() const;
  void RestoreFromMemento(std::shared_ptr<MementoInterface> memento);

  // modify argv match outcome.
  std::vector<Token>::const_iterator GetFirstUnmatchArgument() const;
  std::vector<Token>::const_iterator GetFirstUnmatchArgument(
      std::vector<Token>::const_iterator begin_iter) const;
  std::vector<Token>::const_iterator GetIteratorOfKey(const Token &key) const;

  bool MatchKeyValuePair(
      const Token &key,
      bool (MatchState::*checker)(const Token &) const,
      void (MatchState::*mutator)(const Token &, const Token &));
  bool MatchBooleanKey(const Token &key);
  bool MatchStringKey(const Token &key);
  bool MatchStringListKey(const Token &key);

  const std::vector<Token> tokens_;
  std::multimap<Token, std::vector<Token>::const_iterator> skip_iters_;
  std::shared_ptr<MatchState> match_state_ptr_;
  std::stack<std::shared_ptr<MementoInterface>> memento_stack_;
};

class MatchStrategy : public NodeVisitorInterface {
 public:
  using NodeVisitorInterface::ProcessNode;

  MatchStrategy(const CppCodeGenInfo &info,
                const std::vector<Token> &tokens);
  bool AllMatch() const;
  std::shared_ptr<MatchState> GetMatchStatePtr() const;

  void ProcessNode(LogicAnd::SharedPtr       node) override;
  void ProcessNode(LogicXor::SharedPtr       node) override;
  void ProcessNode(LogicOr::SharedPtr        node) override;
  void ProcessNode(LogicOptional::SharedPtr  node) override;
  void ProcessNode(LogicOneOrMore::SharedPtr node) override;

  void ProcessNode(PosixOption::SharedPtr    node) override;
  void ProcessNode(GnuOption::SharedPtr      node) override;
  void ProcessNode(Argument::SharedPtr       node) override;
  void ProcessNode(Command::SharedPtr        node) override;
  void ProcessNode(GroupedOptions::SharedPtr node) override;

 private:
  MatchStateManager state_manager_;
  bool child_match_condition_ = false;
};

}  // namespace clidoc
#endif  // SRC_CPP11_MATCH_LOGIC_H_
