
#include <string>
#include <vector>
#include <sstream>

#include "ast/generated_scanner.h"
#include "ast/token_proxy.h"
#include "shared/tokenizer.h"

using std::string;
using std::vector;
using std::istringstream;
using std::ostringstream;

namespace clidoc {

vector<Token> FromString(const string &text) {
  // TODO: handle invalid input.
  ostringstream null_ostream;
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

}  // namespace clidoc
