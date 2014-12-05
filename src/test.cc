#include <iostream>
#include <fstream>
#include <sstream>
#include "lex.yy.cc"

using std::cout;
using std::endl;

int main() {
  std::ofstream null_ostream("/dev/null");
  std::istringstream input_stream("docopt -c -afefe --long fuck");
  yyFlexLexer lexer(&input_stream, &null_ostream);
  while(lexer.yylex() != 0) { /* nothing */ }
  for (const auto &token : clidoc::TokenHandler::tokens) {
    cout << token.lexeme_ << " " << token.syntactic_category_ << endl;
  }
  return 0;
}
