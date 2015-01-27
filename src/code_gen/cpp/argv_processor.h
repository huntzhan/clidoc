#ifndef SRC_CODE_GEN_CPP_ARGV_PROCESSOR_H_
#define SRC_CODE_GEN_CPP_ARGV_PROCESSOR_H_

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

#include "ast/ast_node_interface.h"

namespace clidoc {

struct ArgvProcessLogic {
  explicit ArgvProcessLogic(std::string argv) : argv_(argv) { /* empty */ }

  // 1. remove utility name.(done by `ArgvProcessor::LoadArgv`)
  // 2. insert space between keywords(i.e. =, ,).[Guideline 8]
  void InsertSpace();
  // 3. tokenize intput arguments.
  // 4. check binding pairs indicated by =; remove appearance of =.
  void TokenizeArgv();
  // 5. handle --.(reuse #12)[Guideline 10]
  void HandleDoubleHyphen();
  // 6. separate and disambiguate GROUPED_OPTIONS.(recuse #9 and #6)
  //    [Guideline 5, exceptions]
  void HandleGroupedOptions(const std::set<Token> &focused_bound_options);
  // 7. replace each option with its representative option.
  void ReplaceOptionWithRepresentativeOption(
      const std::map<Token, Token> &option_to_rep_option);

  std::string argv_;
  std::list<Token> tokens_;
};

class ArgvProcessor {
 public:
  void LoadArgv(const int &argc, const char **argv);
  std::vector<Token> GetPreprocessedArguments(
      const std::map<Token, Token> &option_to_rep_option,
      const std::set<Token> &focused_bound_options) const;

 private:
  std::string argv_;
};

}  // namespace clidoc
#endif  // SRC_CODE_GEN_CPP_ARGV_PROCESSOR_H_
