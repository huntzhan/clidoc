#include <string>
#include "process_logic.h"

using std::string;

namespace clidoc {

void AmbiguityHandler::ProcessNode(
    GroupedOptions::SharedPtr grouped_options_ptr) {
  // loop over every character of `GROUPED_OPTIONS` and treat each character as
  // `POSIX_OPTION`, say `option`. If `option` is not recorded yet(not appeared
  // in `Options Section`), record it; If `option` is recorded, there are two
  // situations should be considered:
  //   1. `option` is NOT the last character in `GROUPED_OPTIONS`: If `option`
  //   is bound with an argument(i.e. `-o FILE` in `Options Section`), then the
  //   rest of `GROUPED_OPTIONS` would be treated as the argument of `option`,
  //   and those arguments should be exactly matched; If there's no bounded
  //   argument, then the loop would be moved forward.
  //   2. `option` is the last character in `GROUPED_OPTIONS`: nothing happend.
  string value = grouped_options_ptr->token_.value();
  for (auto iter = value.cbegin() + 1;  // ignore prefix `-`.
       iter != value.cend(); ++iter) {
    // build `option` and `left`.
  }
}

}  // namespace clidoc
