#ifndef INCLUDE_CLIDOC_CPP11_H_
#define INCLUDE_CLIDOC_CPP11_H_

#include <map>
#include <vector>
#include <string>

namespace clidoc {

extern std::map<std::string, bool> boolean_outcome;
extern std::map<std::string, std::string> string_outcome;
extern std::map<std::string, std::vector<std::string>> string_list_outcome;

void ParseArguments(const int &argc, const char *const *argv);

}  // namespace clidoc

#endif  // INCLUDE_CLIDOC_CPP11_H_
