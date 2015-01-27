
#include <vector>
#include <set>
#include <map>

#include "code_gen/cpp/match_logic.h"

using std::vector;
using std::string;
using std::map;

namespace clidoc {


explicit MatchStateManager::MatchStateManager(
    const CppCodeGenInfo &info,
    const vector<Token> &tokens)
    : tokens_(tokens) {
  for (auto iter = tokens_.cbegin(); iter != tokens_.cend(); ++iter) {
    const auto &token = *iter;
    // init `skip_iters_`.
    skip_iters_[token] = iter;
    // init `match_state_.token_match_state_`.
    match_state_.token_match_state_[token] = false;
  }
  // init argv match outcome of `match_state_`.
  auto InitBooleanOutcome = [](
      const set<Token> toknes, map<Token, bool> *outcome) {
  };
  auto InitStringOutcome = [](
      const set<Token> toknes, map<Token, string> *outcome) {
  };
  auto InitStringListOutcome = [](
      const set<Token> toknes, map<Token, std::vector<std::string>> *outcome) {
  };
}

}  // namespace clidoc
