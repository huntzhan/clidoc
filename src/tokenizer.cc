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
    case Token::TypeID::POSIX_OPTION:
    case Token::TypeID::GROUPED_OPTIONS:
    case Token::TypeID::GNU_OPTION:
    case Token::TypeID::ARGUEMENT:
    case Token::TypeID::OPERAND:
    case Token::TypeID::OPTION_DEFAULT_VALUE:
      return true;
    default:
      return false;
  }
}

vector<Token> Tokenizer::FromString(const string &text) {
  vector<Token> tokens;
  // load up scanner.
  ofstream null_ostream("/dev/null");
  istringstream input_stream(text);
  FlexGeneratedScanner lexer(&input_stream, &null_ostream);

  while (true) {
    auto item = lexer.lex();
    auto type_id = item.token();
    auto value = item.value.as<string>();
    if (type_id == Token::TypeID::END) {
      break;
    }
    if (TokenHasValue(type_id)) {
      tokens.push_back(Token(type_id, value));
    } else {
      tokens.push_back(Token(type_id));
    }
  }
  return tokens;
}

}  // namespace clTypeIDoc 
