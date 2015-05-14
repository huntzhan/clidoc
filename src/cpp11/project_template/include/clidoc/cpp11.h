#ifndef SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_
#define SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_

#include <cstdint>
#include <map>
#include <vector>
#include <string>

namespace clidoc {

using FlagType = std::uint32_t;

const FlagType SYSTEM_EXIT_OFF = 1 << 0;
const FlagType PRINT_DOC_OFF   = 1 << 1;

// store outcomes.
extern std::map<std::string, bool> boolean_outcome;
extern std::map<std::string, std::string> string_outcome;
extern std::map<std::string, std::vector<std::string>> string_list_outcome;

// input argument processing interface.
void ParseArguments(const int &argc, const char *const *argv);
bool ParseArguments(const int &argc, const char *const *argv,
                    const FlagType &flags);

}  // namespace clidoc

#endif  // SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_
