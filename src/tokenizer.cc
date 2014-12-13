#include "tokenizer.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "generated_scanner.h"
#include "generated_parser.h"
#include "utils.h"

using std::string;
using std::vector;
using std::ofstream;
using std::istringstream;

namespace clidoc {

bool Tokenizer::TokenHasValue(const BisonGeneratedParser::token_type &type) {
  switch (type) {
    case BisonGeneratedParser::token::POSIX_OPTION:
    case BisonGeneratedParser::token::GROUPED_OPTIONS:
    case BisonGeneratedParser::token::GNU_OPTION:
    case BisonGeneratedParser::token::ARGUEMENT:
    case BisonGeneratedParser::token::OPERAND:
    case BisonGeneratedParser::token::OPTION_DEFAULT_VALUE:
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
    auto type = item.token();
    auto value = item.value.as<string>();
    if (type == BisonGeneratedParser::token::END) {
      break;
    }
    if (TokenHasValue(type)) {
      tokens.push_back(Token(type, value));
    } else {
      tokens.push_back(Token(type));
    }
  }
  return tokens;
}

}  // namespace clidoc 
