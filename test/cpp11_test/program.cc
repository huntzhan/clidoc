
#include <iostream>
#include "cpp11/cmake_cpp11_code_gen_project/include/clidoc/cpp11.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  clidoc::ParseArguments(argc, argv);
  for (const auto &map_pair : clidoc::boolean_outcome) {
    cout << map_pair.first << ": ";
    if (map_pair.second) {
      cout << "True";
    } else {
      cout << "False";
    }
    cout << endl;
  }
  for (const auto &map_pair : clidoc::string_outcome) {
    cout << map_pair.first << ": ";
    if (!map_pair.second.empty()) {
      cout << map_pair.second;
    } else {
      cout << "*EMPTY*";
    }
    cout << endl;
  }
  for (const auto &map_pair : clidoc::string_list_outcome) {
    cout << map_pair.first << ": ";
    if (!map_pair.second.empty()) {
      cout << "{ ";
      for (const auto &item : map_pair.second) {
        cout << item << ", ";
      }
      cout << " }";
    } else {
      cout << "*EMPTY*";
    }
    cout << endl;
  }
}
