
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "generated_scanner.h"
#include "node_interface.h"
#include "tokenizer.h"

using std::string;
using std::vector;
using std::ofstream;
using std::istringstream;

#define TokenTypeMapping(name) \
case TypeID::name:             \
  return TerminalType::name    \

namespace clidoc {
namespace tokenizer {

TerminalType ToTerminalType(const Type &type_id) {
  switch (type_id) {
    TokenTypeMapping(K_OPTIONS);
    TokenTypeMapping(K_DOUBLE_HYPHEN);

    TokenTypeMapping(POSIX_OPTION);
    TokenTypeMapping(GROUPED_OPTIONS);
    TokenTypeMapping(GNU_OPTION);
    TokenTypeMapping(ARGUMENT);
    TokenTypeMapping(DEFAULT_VALUE);
    TokenTypeMapping(COMMAND);

    TokenTypeMapping(GENERAL_ELEMENT);
    default:
      return TerminalType::OTHER;
  }
}

bool TokenHasValue(const TerminalType &type) {
  switch (type) {
    case TerminalType::POSIX_OPTION:
    case TerminalType::GROUPED_OPTIONS:
    case TerminalType::GNU_OPTION:

    case TerminalType::ARGUMENT:
    case TerminalType::DEFAULT_VALUE:
    case TerminalType::COMMAND:

    case TerminalType::GENERAL_ELEMENT:
      return true;
    default:
      return false;
  }
}

Token InitToken(const TerminalType &type, const string &value) {
  if (TokenHasValue(type)) {
    return Token(type, value);
  } else {
    return Token(type);
  }
}

Token InitToken(const Type &type_id, const string &value) {
  auto terminal_type = ToTerminalType(type_id);
  return InitToken(terminal_type, value);
}

vector<Token> FromString(const string &text) {
  // TODO: handle invalid input.
  ofstream null_ostream("/dev/null");
  istringstream input_stream(text);
  // Since `FlexGeneratedScanner` accepts istream as argument, lexer->lex()
  // would always return a token with TypeID::END when finished processing
  // `text`.
  FlexGeneratedScanner lexer(&input_stream, &null_ostream);

  vector<Token> tokens;
  while (true) {
    auto item = lexer.lex();
    auto type_id = item.token();
    auto terminal_type = ToTerminalType(type_id);
    auto value = item.value.as<string>();
    if (type_id == TypeID::END) {
      // finish.
      break;
    }
    if (terminal_type == TerminalType::OTHER) {
      // terminals that would not be instaniation.
      continue;
    }
    tokens.push_back(InitToken(terminal_type, value));
  }
  return tokens;
}

}  // namespace clidoc::tokenizer
}  // namespace clidoc
