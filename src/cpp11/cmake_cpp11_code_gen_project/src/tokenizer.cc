
#include <string>
#include <vector>
#include <sstream>

// TODO(huntzhan): remove dependencies of ast.
#include "ast/ast_node_interface.h"
#include "ast/generated_scanner.h"
#include "tokenizer.h"

using std::string;
using std::vector;
using std::istringstream;
using std::ostringstream;

// I know this is ugly. Fix it if you have a better plan.
#define TokenTypeMapping(name) \
case TypeID::name:             \
  return TerminalType::name    \

// facilitate usage.
using Type = clidoc::BisonGeneratedParser::token_type;
using TypeID = clidoc::BisonGeneratedParser::token;

namespace clidoc {

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

vector<Token> FromString(const string &text) {
  // TODO(huntzhan): handle invalid input.
  istringstream input_stream(text);
  ostringstream null_ostream;
  // Since `FlexGeneratedScanner` accepts istream as argument, lexer->lex()
  // would always return a token with TypeID::END when finished processing
  // `text`.
  FlexGeneratedScanner lexer(&input_stream, &null_ostream);

  vector<Token> tokens;
  while (true) {
    auto item = lexer.lex();
    auto type_id = item.token();
    auto terminal_type = ToTerminalType(type_id);
    if (type_id == TypeID::END) {
      // finish.
      break;
    }
    if (terminal_type == TerminalType::OTHER) {
      // terminals that would not be instaniation.
      continue;
    }
    // add token.
    if (TokenHasValue(terminal_type)) {
      const string &value = item.value.as<string>();
      tokens.emplace_back(terminal_type, value);
    } else {
      tokens.emplace_back(terminal_type);
    }
  }
  return tokens;
}

}  // namespace clidoc
