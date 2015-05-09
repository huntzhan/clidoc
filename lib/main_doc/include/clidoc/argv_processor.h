#ifndef SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_ARGV_PROCESSOR_H_
#define SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_ARGV_PROCESSOR_H_

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

#include "clidoc/ast/ast_node_interface.h"

namespace clidoc {

struct ArgvProcessLogic {
  enum class ArgumentPattern {
    POSIX_OPTION,
    GNU_OPTION,
    SINGLE_DASH_CASE,
    DOUBLE_DASH_CASE,
    UNKNOW_CASE,
  };

  explicit ArgvProcessLogic(
      const std::vector<std::string> &argv,
      const std::map<Token, Token> &option_to_rep_option,
      const std::set<Token> &bound_options);

  ArgumentPattern DetectArgumentPattern(const std::string &argument);
  bool ReplaceWithRepresentativeOption(Token *option_ptr);
  bool OptionIsBound(const Token &option);

  void TokenizeArgv();

  // return True indicates skipping next input argument.
  bool ProcessOption(const TerminalType &type, const std::string &argument);
  bool ProcessSingleDashCase(const std::string &argument);
  bool ProcessDoubleDashCase(const std::string &argument);
  // return True indicates detecting `--`.
  bool ProcessUnknowCase(const std::string &argument);

  const std::vector<std::string> &argv_;
  const std::map<Token, Token> &option_to_rep_option_;
  const std::set<Token> &bound_options_;
  std::list<Token> tokens_;
};

class ArgvProcessor {
 public:
  void LoadArgv(const int &argc, const char *const *argv);
  std::vector<Token> GetPreprocessedArguments(
      const std::map<Token, Token> &option_to_rep_option,
      const std::set<Token> &bound_options) const;

 private:
  std::vector<std::string> argv_;
};

}  // namespace clidoc
#endif  // SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_ARGV_PROCESSOR_H_
