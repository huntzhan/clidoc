
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/cpp11.h"
#include "clidoc/info.h"
#include "clidoc/argv_processor.h"
#include "clidoc/match_logic.h"

using std::bitset;
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

bool ParseArguments(const int &argc, const char *const *argv,
                    const FlagType &flags) {
  const bitset<32> bit_flags = bitset<32>(~0ULL) & bitset<32>(~flags);
  const bool system_exit_on =
      bit_flags.test(static_cast<size_t>(Flag::SYSTEM_EXIT_OFF));
  const bool print_doc_on =
      bit_flags.test(static_cast<size_t>(Flag::PRINT_DOC_OFF));

  auto RespondToError = [&]() -> bool {
    if (print_doc_on) {
      cout << cpp_code_gen_info.doc_text_ << endl;
    }
    if (system_exit_on) {
      exit(0);
    }
    return false;
  };

  // clear outcomes.
  boolean_outcome.clear();
  string_outcome.clear();
  string_list_outcome.clear();

  // tokenize input arguments.
  ArgvProcessor argv_processor;
  argv_processor.LoadArgv(argc, argv);
  auto tokens = argv_processor.GetPreprocessedArguments(
      cpp_code_gen_info.option_to_representative_option_,
      cpp_code_gen_info.bound_options_);
  if (tokens.empty()) {
    return RespondToError();
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
    return RespondToError();
  }
  return true;
}

void ParseArguments(const int &argc, const char *const *argv) {
  ParseArguments(argc, argv, 0ULL);
}

}  // namespace clidoc
