#include <iostream>
#include <fstream>
#include <sstream>
#include "generated_scanner.h"

using std::cout;
using std::endl;

int main() {
  std::ofstream null_ostream("/dev/null");
  std::istringstream input_stream("docopt -c -afefe --long \"bitch\" -c=a b c <shit> FILE fuck");
  clidoc::FlexGeneratedScanner lexer(&input_stream, &null_ostream);
  while (true) {
    auto token = lexer.lex();
    if (token.token() == 0) {
      break;
    }
    cout << token.token() << " " << token.value.as<std::string>() << endl;
  }
  return 0;
}
