
#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include "clidoc/cpp11.h"

using std::cout;
using std::endl;
using std::function;
using std::string;
using std::vector;

template <typename OutcomeType, typename Lambda>
void PrintOutcome(const OutcomeType &outcome, const Lambda &lambda) {
  for (const auto &map_pair : outcome) {
    cout << map_pair.first << ": ";
    lambda(map_pair.second);
    cout << endl;
  }
}

int main(int argc, char *argv[]) {
  clidoc::ParseArguments(argc, argv);

  auto PrintBoolean = [](const bool &flag){
    cout << (flag? "True" : "False");
  };

  auto PrintString = [](const string &text){
    cout << (text.empty()? "*EMPTY*" : text);
  };

  auto PrintStringList = [](const vector<string> &texts){
    cout << "[ ";
    for (const auto &text : texts) {
      cout << "\"" << text << "\" ";
    }
    cout << "]";
  };

  PrintOutcome(clidoc::boolean_outcome, PrintBoolean);
  PrintOutcome(clidoc::string_outcome, PrintString);
  PrintOutcome(clidoc::string_list_outcome, PrintStringList);
}
