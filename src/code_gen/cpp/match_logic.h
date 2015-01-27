#ifndef SRC_CODE_GEN_CPP_MATCH_LOGIC_H_
#define SRC_CODE_GEN_CPP_MATCH_LOGIC_H_

#include <stack>
#include <memory>
#include <map>
#include <string>
#include <vector>

#include "ast/ast_node_interface.h"
#include "code_gen/cpp/info.h"

namespace clidoc {

struct MatchState {
  // input arguments.
  std::map<Token, bool> token_match_state_;
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
  MatchState match_state_;
};

class MatchStateManager {
 public:
  MatchStateManager(
      const CppCodeGenInfo &info,
      const std::vector<Token> &tokens);
  void PushRollbackPoint();
  void PopRollbackPoint();
  void Rollback();

 private:
  friend class SimpleMemento;
  std::shared_ptr<MementoInterface> CreateMemento() const;
  void RestoreFromMemento(std::shared_ptr<MementoInterface> memento);

  const std::vector<Token> &tokens_;
  std::map<Token, std::vector<Token>::const_iterator> skip_iters_;
  MatchState match_state_;
  std::stack<std::shared_ptr<MementoInterface>> memento_stack_;
};

}  // namespace clidoc
#endif  // SRC_CODE_GEN_CPP_MATCH_LOGIC_H_
