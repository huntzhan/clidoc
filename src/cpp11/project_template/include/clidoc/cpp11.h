#ifndef SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_
#define SRC_CPP11_PROJECT_TEMPLATE_INCLUDE_CLIDOC_CPP11_H_

// user can directly use something like
// `clidoc::FORBID_EXIT | clidoc::HANDLE_HELP_OPTION` to control the behaviour
// of argument processing.
#define GENERATE_FLAG(name) \
  const FlagType name = 1 << static_cast<int>(Flag::name)

#include <cstddef>
#include <map>
#include <vector>
#include <string>

namespace clidoc {

using FlagType = unsigned long long;

enum class Flag : std::size_t {
  SYSTEM_EXIT_OFF = 0,
  PRINT_DOC_OFF ,
};

GENERATE_FLAG(SYSTEM_EXIT_OFF);
GENERATE_FLAG(PRINT_DOC_OFF);

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
