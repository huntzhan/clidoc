#include <algorithm>
#include <cctype>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/argv_processor.h"

using std::back_inserter;
using std::copy;
using std::isalnum;
using std::list;
using std::map;
using std::set;
using std::string;
using std::vector;

namespace clidoc {

ArgvProcessLogic::ArgvProcessLogic(
    const vector<string> &argv,
    const map<Token, Token> &option_to_rep_option,
    const set<Token> &bound_options)
    :
    argv_(argv),
    option_to_rep_option_(option_to_rep_option),
    bound_options_(bound_options) { /* empty */ }

ArgvProcessLogic::ArgumentPattern
ArgvProcessLogic::DetectArgumentPattern(const string &argument) {
  auto isdash = [](int ch) -> bool {
    return ch == '-';
  };

  if (argument.size() < 2) {
    return ArgumentPattern::UNKNOW_CASE;
  }
  // state: `argument.size()` >= 2.
  if (!isdash(argument[0])) {
    return ArgumentPattern::UNKNOW_CASE;
  }
  // state: `argument` starts with dash.
  // test POSIX_OPTION.
  if (argument.size() == 2) {
    if (isalnum(argument[1])) {
      return ArgumentPattern::POSIX_OPTION;
    }
    return ArgumentPattern::UNKNOW_CASE;
  }
  // state: `argument.size()` > 2 and `argument` starts with dash.
  // test GNU_OPTION.
  bool second_char_is_dash = isdash(argument[1]);
  bool third_char_is_alnum = isalnum(argument[2]);
  bool fit_gnu_option = true;
  for (auto char_iter = argument.cbegin() + 3;
       char_iter != argument.cend(); ++char_iter) {
    if (isalnum(*char_iter) || isdash(*char_iter)) {
      continue;
    }
    fit_gnu_option = false;
    break;
  }
  if (second_char_is_dash && third_char_is_alnum && fit_gnu_option) {
    return ArgumentPattern::GNU_OPTION;
  }
  // state: `argument.size()` > 2,
  //        `argument` starts with dash,
  //        `argument` is not a GNU_OPTION.
  // test prefix.
  if (second_char_is_dash) {
    return ArgumentPattern::DOUBLE_DASH_CASE;
  } else {
    return ArgumentPattern::SINGLE_DASH_CASE;
  }
}

bool
ArgvProcessLogic::ReplaceWithRepresentativeOption(Token *option_ptr) {
  if (option_to_rep_option_.find(*option_ptr)
      != option_to_rep_option_.cend()) {
    *option_ptr = option_to_rep_option_.at(*option_ptr);
    return true;
  }
  return false;
}

bool ArgvProcessLogic::OptionIsBound(const Token &option) {
  return bound_options_.find(option) != bound_options_.cend();
}

void ArgvProcessLogic::TokenizeArgv() {
  auto iter = argv_.cbegin();
  bool flag = (iter != argv_.cend());
  bool skip_next_argument = false;
  while (flag) {
    const string &argument = *iter;
    switch (DetectArgumentPattern(argument)) {
      case ArgumentPattern::POSIX_OPTION:
        skip_next_argument =
            ProcessOption(TerminalType::POSIX_OPTION, argument);
        break;
      case ArgumentPattern::GNU_OPTION:
        skip_next_argument = ProcessOption(TerminalType::GNU_OPTION, argument);
        break;
      case ArgumentPattern::SINGLE_DASH_CASE:
        skip_next_argument = ProcessSingleDashCase(argument);
        break;
      case ArgumentPattern::DOUBLE_DASH_CASE:
        skip_next_argument = ProcessDoubleDashCase(argument);
        break;
      case ArgumentPattern::UNKNOW_CASE:
        if (ProcessUnknowCase(argument)) {
          // if detected `--`, stop looping.
          flag = false;
        }
        break;
    }
    if (skip_next_argument) {
      ++iter;
      tokens_.push_back(
          Token(TerminalType::GENERAL_ELEMENT, *iter));
      // reset.
      skip_next_argument = false;
    }
    // 1. since increment end() results in undefined behaviour, check it before
    // self-increment.
    // 2. in case of `argument == "--"`, self-increment force `iter` points to
    // next input argument.
    if (iter != argv_.cend()) {
      ++iter;
    }
    // if `--` is detected, `flag` would be false. otherwise, `flag` depends on
    // boundary check.
    flag = flag && iter != argv_.cend();
  }
  // handle arguments after `--`.
  for (; iter != argv_.cend(); ++iter) {
    tokens_.push_back(
        Token(TerminalType::GENERAL_ELEMENT, *iter));
  }
}

bool ArgvProcessLogic::ProcessOption(
    const TerminalType &type, const string &argument) {
  Token option(type, argument);
  if (!ReplaceWithRepresentativeOption(&option)) {
    option.set_type(TerminalType::GENERAL_ELEMENT);
  }
  tokens_.push_back(option);
  if (OptionIsBound(option)) {
    return true;
  }
  return false;
}

bool ArgvProcessLogic::ProcessSingleDashCase(const string &argument) {
  const string dash("-");

  vector<Token> tokens_cache;
  bool merge_flag = true;
  bool skip_next_argument = false;

  for (auto iter = argument.cbegin() + 1;  // ignore "-".
       iter != argument.cend(); ++iter) {
    Token option(TerminalType::POSIX_OPTION, dash + *iter);
    if (!ReplaceWithRepresentativeOption(&option)) {
      // `option` is invalid.
      merge_flag = false;
      break;
    }
    // record it.
    tokens_cache.push_back(option);
    // check bound or not.
    if (OptionIsBound(option)) {
      if (iter < argument.cend() - 1) {
        // not the last character.
        Token rest(TerminalType::GENERAL_ELEMENT,
                   string(iter + 1, argument.cend()));
        tokens_cache.push_back(rest);
        // done processing.
        break;
      } else {
        // last character.
        skip_next_argument = true;
      }
    }
  }

  if (merge_flag) {
    copy(tokens_cache.begin(), tokens_cache.end(), back_inserter(tokens_));
  } else {
    // cannot split `argument`, treat it as `GENERAL_ELEMENT`.
    tokens_.push_back(
        Token(TerminalType::GENERAL_ELEMENT, argument));
  }
  return skip_next_argument;
}

bool ArgvProcessLogic::ProcessDoubleDashCase(const string &argument) {
  bool split_flag = false;

  auto equal_sign_index = argument.find('=');
  while (equal_sign_index != string::npos) {
    // state: --...aaa=bbb...
    //        0       ^ [equal_sign_index]
    Token gnu_option(TerminalType::GNU_OPTION,
                     argument.substr(0, equal_sign_index));
    if (ReplaceWithRepresentativeOption(&gnu_option)
        && equal_sign_index != argument.size() - 1) {
      // `gnu_option` is valid and `=` is not the last character.
      Token rest(TerminalType::GENERAL_ELEMENT,
                 argument.substr(equal_sign_index + 1));
      tokens_.push_back(gnu_option);
      tokens_.push_back(rest);
      split_flag = true;
      break;
    }
    // current split is invalid, try next.
    equal_sign_index = argument.find('=', equal_sign_index + 1);
  }
  if (!split_flag) {
    tokens_.push_back(
        Token(TerminalType::GENERAL_ELEMENT, argument));
  }
  return false;
}

bool ArgvProcessLogic::ProcessUnknowCase(const string &argument) {
  if (argument == "--") {
    return true;
  }
  tokens_.push_back(
      Token(TerminalType::GENERAL_ELEMENT, argument));
  return false;
}

void ArgvProcessor::LoadArgv(const int &argc, const char *const *argv) {
  for (int index = 1; index < argc; ++index) {
    argv_.push_back(argv[index]);
  }
}

vector<Token> ArgvProcessor::GetPreprocessedArguments(
    const map<Token, Token> &option_to_rep_option,
    const set<Token> &bound_options) const {
  ArgvProcessLogic process_logic(argv_, option_to_rep_option, bound_options);
  process_logic.TokenizeArgv();
  return vector<Token>(
      process_logic.tokens_.begin(),
      process_logic.tokens_.end());
}

}  // namespace clidoc
