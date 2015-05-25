#ifndef SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_ARGV_PROCESSOR_H_
#define SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_ARGV_PROCESSOR_H_

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/info.h"

namespace clidoc {

struct ArgvProcessLogic {
  enum class ArgumentPattern {
    POSIX_OPTION,
    GNU_OPTION,
    SINGLE_DASH_CASE,
    DOUBLE_DASH_CASE,
    UNKNOW_CASE,
  };

  ArgvProcessLogic(
      const std::vector<std::string> &argv,
      const CppCodegenInfo &info);

  ArgumentPattern DetectArgumentPattern(const std::string &argument);
  bool ReplaceWithRepresentativeOption(Token *option_ptr);
  bool OptionIsBound(const Token &option);
  bool ArgumentIsCommand(const std::string &argument);

  void TokenizeArgv();
  void FillTokens();
  void CorrectOOMArgumentType();

  // return True indicates skipping next input argument.
  bool ProcessOption(const TerminalType &type, const std::string &argument);
  bool ProcessSingleDashCase(const std::string &argument);
  bool ProcessDoubleDashCase(const std::string &argument);
  // return True indicates detecting `--`.
  bool ProcessUnknowCase(const std::string &argument);

  const std::vector<std::string> &argv_;
  const CppCodegenInfo &info_;
  std::list<Token> tokens_;
};

class ArgvProcessor {
 public:
  void LoadArgv(const int &argc, const char *const *argv);
  std::vector<Token> GetPreprocessedArguments(
      const CppCodegenInfo &info) const;

 private:
  std::vector<std::string> argv_;
};

}  // namespace clidoc
#endif  // SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_ARGV_PROCESSOR_H_
