#ifndef SRC_CODE_GEN_CPP_ARGV_PREPROCESSOR_H_
#define SRC_CODE_GEN_CPP_ARGV_PREPROCESSOR_H_

#include <string>
#include <vector>
#include <list>
#include <set>

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

  std::string argv_;
  std::list<Token> tokens_;
};

class ArgvProcessor {
 public:
  void LoadArgv(const int &argc, const char **argv);
  std::vector<Token> GetPreprocessedArguments(
      const std::set<Token> &focused_bound_options) const;

 private:
  std::string argv_;
};

}  // namespace clidoc
#endif  // SRC_CODE_GEN_CPP_ARGV_PREPROCESSOR_H_
