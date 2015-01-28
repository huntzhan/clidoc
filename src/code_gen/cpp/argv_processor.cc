
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "ast/ast_node_interface.h"
#include "ast/string_utils.h"
#include "ast/tokenizer.h"
#include "code_gen/cpp/argv_processor.h"

using std::string;
using std::vector;
using std::list;
using std::set;
using std::find;
using std::next;
using std::copy;
using std::back_inserter;
using std::map;

namespace clidoc {

ArgvProcessLogic::ArgvProcessLogic(const vector<string> &argv)
    : argv_(argv) { /* empty */ }

void ArgvProcessLogic::TokenizeArgv() {
  auto ArgIsNotSeperable = [](const string &arg) -> bool {
    if (arg.find(' ') != string::npos) {
      return true;
    }
    if (arg.find('\t') != string::npos) {
      return true;
    }
    return false;
  };
  auto ProcessText = [&](string *text_ptr) {
    if (!text_ptr->empty()) {
      StringUtils::InsertSpace(
          {},          // no exclude pattern yet.
          {"=", ","},  // insert space arround `=` and `,`.
          text_ptr);
      auto tokens = FromString(*text_ptr);
      copy(tokens.begin(), tokens.end(), back_inserter(tokens_));
      text_ptr->clear();
    }
  };

  string text;
  for (auto iter = argv_.cbegin(); iter != argv_.cend(); ++iter) {
    const string &arg = *iter;
    // handle --.(reuse #12)[Guideline 10]
    if (arg == "--") {
      // process string before `--`.
      ProcessText(&text);
      // add elements after `--`, including `--`.
      tokens_.push_back(Token(TerminalType::K_DOUBLE_HYPHEN));
      for (auto inner_iter = next(iter);
           inner_iter != argv_.cend(); ++inner_iter) {
        tokens_.push_back(
            Token(TerminalType::GENERAL_ELEMENT, *inner_iter));
      }
      break;
    }
    // not a quoted unit, append to `text`.
    if (!ArgIsNotSeperable(arg)) {
      text.append(arg);
      text.push_back(' ');
      continue;
    }
    // arg contains space.
    // 1. process `text`.
    ProcessText(&text);
    // 2. add `arg` as `GENERAL_ELEMENT`.
    tokens_.push_back(Token(TerminalType::GENERAL_ELEMENT, arg));
  }
  ProcessText(&text);
}

void ArgvProcessLogic::HandleGroupedOptions(
    const set<Token> &focused_bound_options) {
  auto end_iter = find(tokens_.begin(), tokens_.end(),
                       Token(TerminalType::K_DOUBLE_HYPHEN));
  if (end_iter != tokens_.end()) {
    // remove `--`.
    end_iter = tokens_.erase(end_iter);
  }
  for (auto token_iter = tokens_.begin();
       token_iter != end_iter; ++token_iter) {
    // consider two kinds of tokens:
    // 1. `GENERAL_ELEMENT` starts with `-`.
    // 2. `GROUPED_OPTIONS`.
    if (token_iter->type() == TerminalType::GENERAL_ELEMENT) {
      auto value = token_iter->value();
      if (value.front() != '-') {
        continue;
      }
    } else if (token_iter->type() != TerminalType::GROUPED_OPTIONS) {
      continue;
    }
    auto value = token_iter->value();
    for (auto char_iter = value.begin() + 1;
         char_iter != value.end(); ++char_iter) {
      auto option = Token(TerminalType::POSIX_OPTION,
                          string("-") + *char_iter);
      tokens_.insert(token_iter, option);
      if (focused_bound_options.find(option) == focused_bound_options.end()) {
        // option not bound.
        continue;
      } else {
        string option_argument_value(char_iter + 1, value.end());
        auto option_argument = Token(
            TerminalType::GENERAL_ELEMENT,
            option_argument_value);
        if (!option_argument_value.empty()) {
          tokens_.insert(token_iter, option_argument);
        }
        break;
      }
    }
    // remove original token.
    token_iter = tokens_.erase(token_iter);
    --token_iter;
  }
}

void ArgvProcessLogic::ReplaceOptionWithRepresentativeOption(
    const map<Token, Token> &option_to_rep_option) {
  auto iter = tokens_.begin();
  while (iter != tokens_.end()) {
    if (option_to_rep_option.find(*iter) != option_to_rep_option.end()) {
      tokens_.insert(iter, option_to_rep_option.at(*iter));
      iter = tokens_.erase(iter);
      continue;
    }
    ++iter;
  }
}

void ArgvProcessor::LoadArgv(const int &argc, const char **argv) {
  for (int index = 1; index != argc; ++index) {
    argv_.push_back(argv[index]);
  }
}

vector<Token> ArgvProcessor::GetPreprocessedArguments(
    const std::map<Token, Token> &option_to_rep_option,
    const set<Token> &focused_bound_options) const {
  ArgvProcessLogic process_logic(argv_);
  process_logic.TokenizeArgv();
  process_logic.HandleGroupedOptions(focused_bound_options);
  process_logic.ReplaceOptionWithRepresentativeOption(option_to_rep_option);
  return vector<Token>(
      process_logic.tokens_.begin(),
      process_logic.tokens_.end());
}

}  // namespace clidoc
