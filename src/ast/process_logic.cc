#include <string>

#include "ast/ast_node_interface.h"
#include "ast/token_proxy.h"
#include "ast/process_logic.h"

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

  auto &o2ro = recorder_ptr_->option_to_representative_option_;
  auto &ro2pty = recorder_ptr_->representative_option_to_property_;

  // `-hso` -> `-h -s -o`.
  auto logic_and = LogicAnd::Init();

  string value = grouped_options_ptr->token_.value();
  for (auto iter = value.cbegin() + 1;  // ignore prefix `-`.
       iter != value.cend(); ++iter) {
    // build `option` and `remain`.
    auto option = InitToken(TerminalType::POSIX_OPTION,
                            "-" + string(iter, iter + 1));
    // add `option`.
    auto posix_option_node = PosixOption::Init(option);
    logic_and->AddChild(posix_option_node);

    if (o2ro.find(option) == o2ro.end()) {
      // `option` not recorded.
      recorder_ptr_->RecordBinding(option, Token());
      continue;
    } 

    auto representative_option = o2ro[option];
    if (ro2pty[representative_option].IsEmpty()) {
      // `option` not bound with argument.
      continue;
    }

    auto remain = InitToken(TerminalType::ARGUMENT,
                            string(iter + 1, value.cend()));
    if (!remain.IsEmpty()) {
      // add `remain`.
      auto argument_node = Argument::Init(remain);
      logic_and->AddChild(argument_node);
      break;
    }
  }
  // replace original `GroupedOptions`.
  SharedPtrNode &ptr_in_parent =
      *grouped_options_ptr->node_connection.this_child_ptr_;
  ptr_in_parent = logic_and;
}

}  // namespace clidoc
