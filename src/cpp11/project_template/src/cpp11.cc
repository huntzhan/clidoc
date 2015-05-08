
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/cpp11.h"
#include "clidoc/info.h"
#include "clidoc/argv_processor.h"
#include "clidoc/match_logic.h"

using std::string;
using std::map;
using std::vector;
using std::cout;
using std::endl;
using std::exit;

namespace clidoc {

map<string, bool> boolean_outcome{};
map<string, string> string_outcome{};
map<string, vector<string>> string_list_outcome{};

template <typename ValueType>
void AssignOutcome(
    const map<Token, ValueType> &source,
    map<string, ValueType> *destination) {
  for (const auto &map_pair : source) {
    (*destination)[map_pair.first.value()] = map_pair.second;
  }
}

void ParseArguments(const int &argc, const char *const *argv) {
  // tokenize input arguments.
  ArgvProcessor argv_processor;
  argv_processor.LoadArgv(argc, argv);
  auto tokens = argv_processor.GetPreprocessedArguments(
      cpp_code_gen_info.option_to_representative_option_,
      cpp_code_gen_info.bound_options_);
  if (tokens.empty()) {
    cout << cpp_code_gen_info.doc_text_ << endl;
    exit(0);
  }

  // analyse input arguments.
  MatchStrategy match_strategy(cpp_code_gen_info, tokens);
  cpp_code_gen_info.doc_node_->Accept(&match_strategy);

  if (match_strategy.AllMatch()) {
    auto match_state_ptr = match_strategy.GetMatchStatePtr();
    AssignOutcome(match_state_ptr->boolean_outcome_, &boolean_outcome);
    AssignOutcome(match_state_ptr->string_outcome_, &string_outcome);
    AssignOutcome(match_state_ptr->string_list_outcome_, &string_list_outcome);
  } else {
    cout << cpp_code_gen_info.doc_text_ << endl;
    exit(0);
  }
}

}  // namespace clidoc
