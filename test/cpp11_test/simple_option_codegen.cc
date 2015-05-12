#include "clidoc/info.h"
namespace clidoc {
CppCodeGenInfo InitCppCodeGenInfo() {
CppCodeGenInfo cpp_code_gen_info;
cpp_code_gen_info.bound_options_ = {
};
cpp_code_gen_info.unbound_options_ = {
  Token(TerminalType::GNU_OPTION, "--long-1"),
  Token(TerminalType::POSIX_OPTION, "-a"),
};
cpp_code_gen_info.arguments_ = {
};
cpp_code_gen_info.oom_bound_options_ = {
};
cpp_code_gen_info.oom_arguments_ = {
};
cpp_code_gen_info.commands_ = {
};
cpp_code_gen_info.default_values_ = {
};
cpp_code_gen_info.option_to_representative_option_ = {
  {Token(TerminalType::GNU_OPTION, "--long-1"), Token(TerminalType::GNU_OPTION, "--long-1")},
  {Token(TerminalType::POSIX_OPTION, "-a"), Token(TerminalType::POSIX_OPTION, "-a")},
};
auto t0 = PosixOption::Init(Token(TerminalType::POSIX_OPTION, "-a"));
auto t1 = GnuOption::Init(Token(TerminalType::GNU_OPTION, "--long-1"));
auto nt2 = LogicXor::Init();
nt2->AddChild(t0);
nt2->AddChild(t1);
auto nt5 = Doc::Init();
nt5->AddChild(nt2);
cpp_code_gen_info.doc_node_ = nt5;
cpp_code_gen_info.doc_text_ = R"doc(Usage:
  utility_name -a
  utility_name --long-1
)doc";
return cpp_code_gen_info;
}
CppCodeGenInfo cpp_code_gen_info = InitCppCodeGenInfo();
}  // namespace clidoc