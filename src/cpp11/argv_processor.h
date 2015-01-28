#ifndef SRC_CPP11_ARGV_PROCESSOR_H_
#define SRC_CPP11_ARGV_PROCESSOR_H_

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

#include "ast/ast_node_interface.h"

namespace clidoc {

struct ArgvProcessLogic {
  explicit ArgvProcessLogic(const std::vector<std::string> &argv);

  void TokenizeArgv();
  void HandleGroupedOptions(const std::set<Token> &focused_bound_options);
  void ReplaceOptionWithRepresentativeOption(
      const std::map<Token, Token> &option_to_rep_option);

  const std::vector<std::string> &argv_;
  std::list<Token> tokens_;
};

class ArgvProcessor {
 public:
  void LoadArgv(const int &argc, const char **argv);
  std::vector<Token> GetPreprocessedArguments(
      const std::map<Token, Token> &option_to_rep_option,
      const std::set<Token> &focused_bound_options) const;

 private:
  std::vector<std::string> argv_;
};

}  // namespace clidoc
#endif  // SRC_CPP11_ARGV_PROCESSOR_H_
