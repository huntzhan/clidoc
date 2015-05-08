#ifndef SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_
#define SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_

#include <map>
#include <vector>
#include <string>

namespace clidoc {

extern std::map<std::string, bool> boolean_outcome;
extern std::map<std::string, std::string> string_outcome;
extern std::map<std::string, std::vector<std::string>> string_list_outcome;

void ParseArguments(const int &argc, const char *const *argv);

}  // namespace clidoc

#endif  // SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_
