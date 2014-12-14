#include "tokenizer.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "generated_scanner.h"
#include "utils.h"

using std::string;
using std::vector;
using std::ofstream;
using std::istringstream;

namespace clidoc {

inline bool Tokenizer::TokenHasValue(const Token::Type &type_id) {
  switch (type_id) {
    // Only for doc processing.
    case Token::TypeID::OPTION_ARGUEMENT:
    case Token::TypeID::GNU_OPTION:
    case Token::TypeID::OPERAND:
    case Token::TypeID::COMMENT:
    // Only for argument processing.
    case Token::TypeID::ARGUMENT:
    // Shared.
    case Token::TypeID::OPTION_DEFAULT_VALUE:
    case Token::TypeID::POSIX_OPTION:
    case Token::TypeID::GROUPED_OPTIONS:
      return true;
    default:
      return false;
  }
}

vector<Token> Tokenizer::FromString(const string &text) {
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
    auto value = item.value.as<string>();
    if (type_id == Token::TypeID::END) { break; }
    if (TokenHasValue(type_id)) {
      tokens.push_back(Token(type_id, value));
    } else {
      tokens.push_back(Token(type_id));
    }
  }
  return tokens;
}

}  // namespace clTypeIDoc 
