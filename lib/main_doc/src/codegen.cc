#include "clidoc/info.h"
namespace clidoc {
CppCodeGenInfo InitCppCodeGenInfo() {
CppCodeGenInfo cpp_code_gen_info;
cpp_code_gen_info.bound_options_ = {
  Token(TerminalType::GNU_OPTION, "--mode"),
};
cpp_code_gen_info.unbound_options_ = {
  Token(TerminalType::GNU_OPTION, "--debug"),
  Token(TerminalType::GNU_OPTION, "--list-mode"),
};
cpp_code_gen_info.arguments_ = {
  Token(TerminalType::ARGUMENT, "<doc_name>"),
  Token(TerminalType::ARGUMENT, "<output_hint>"),
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
  {Token(TerminalType::GNU_OPTION, "--debug"), Token(TerminalType::GNU_OPTION, "--debug")},
  {Token(TerminalType::GNU_OPTION, "--list-mode"), Token(TerminalType::GNU_OPTION, "--list-mode")},
  {Token(TerminalType::GNU_OPTION, "--mode"), Token(TerminalType::GNU_OPTION, "--mode")},
  {Token(TerminalType::POSIX_OPTION, "-m"), Token(TerminalType::GNU_OPTION, "--mode")},
};
auto t0 = GnuOption::Init(Token(TerminalType::GNU_OPTION, "--mode"));
auto t1 = PosixOption::Init(Token(TerminalType::POSIX_OPTION, "-m"));
auto nt2 = LogicXor::Init();
nt2->AddChild(t0);
nt2->AddChild(t1);
auto t5 = Argument::Init(Token(TerminalType::ARGUMENT, "<doc_name>"));
auto t6 = Argument::Init(Token(TerminalType::ARGUMENT, "<output_hint>"));
auto nt7 = LogicAnd::Init();
nt7->AddChild(nt2);
nt7->AddChild(t5);
nt7->AddChild(t6);
auto t11 = GnuOption::Init(Token(TerminalType::GNU_OPTION, "--list-mode"));
auto t12 = GnuOption::Init(Token(TerminalType::GNU_OPTION, "--debug"));
auto t13 = Argument::Init(Token(TerminalType::ARGUMENT, "<doc_name>"));
auto nt14 = LogicAnd::Init();
nt14->AddChild(t12);
nt14->AddChild(t13);
auto nt17 = LogicXor::Init();
nt17->AddChild(nt7);
nt17->AddChild(t11);
nt17->AddChild(nt14);
auto nt21 = Doc::Init();
nt21->AddChild(nt17);
cpp_code_gen_info.doc_node_ = nt21;
cpp_code_gen_info.doc_text_ = R"doc(Usage:
  clidoc_main --mode|-m <mode> <doc_name> <output_hint>
  clidoc_main --list-mode
  clidoc_main --debug <doc_name>

Options:
  --mode=<mode>, -m <mode>  Set mode of code gen.
  <doc_name>                The filename of user defined doc.
  <output_hint>             A string to guide code gen.
  --list-mode               List avaliable modes.
  --debug                   List information for debug.
)doc";
return cpp_code_gen_info;
}
CppCodeGenInfo cpp_code_gen_info = InitCppCodeGenInfo();
}  // namespace clidoc