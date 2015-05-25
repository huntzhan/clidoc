
#include "clidoc/info.h"
namespace clidoc {
CppCodegenInfo InitCppCodegenInfo() {
CppCodegenInfo cpp_codegen_info;
cpp_codegen_info.bound_options_ = {
Token(TerminalType::GNU_OPTION, "--mode"),
};
cpp_codegen_info.unbound_options_ = {
Token(TerminalType::GNU_OPTION, "--debug"),
Token(TerminalType::GNU_OPTION, "--list-mode"),
};
cpp_codegen_info.arguments_ = {
Token(TerminalType::ARGUMENT, "<doc_name>"),
Token(TerminalType::ARGUMENT, "<output_hint>"),
};
cpp_codegen_info.oom_bound_options_ = {
};
cpp_codegen_info.oom_arguments_ = {
};
cpp_codegen_info.commands_ = {
};
cpp_codegen_info.default_values_ = {
};
cpp_codegen_info.option_to_representative_option_ = {
{Token(TerminalType::GNU_OPTION, "--debug"), Token(TerminalType::GNU_OPTION, "--debug")},
{Token(TerminalType::GNU_OPTION, "--list-mode"), Token(TerminalType::GNU_OPTION, "--list-mode")},
{Token(TerminalType::GNU_OPTION, "--mode"), Token(TerminalType::GNU_OPTION, "--mode")},
{Token(TerminalType::POSIX_OPTION, "-m"), Token(TerminalType::GNU_OPTION, "--mode")},
};
cpp_codegen_info.doc_text_ = R"doc(Usage:
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
auto node_0 = GnuOption::Init("--mode");
auto node_1 = PosixOption::Init("-m");
auto node_2 = LogicXor::Init();
node_2->AddChild(node_0);
node_2->AddChild(node_1);
auto node_3 = Argument::Init("<doc_name>");
auto node_4 = Argument::Init("<output_hint>");
auto node_5 = LogicAnd::Init();
node_5->AddChild(node_2);
node_5->AddChild(node_3);
node_5->AddChild(node_4);
auto node_6 = GnuOption::Init("--list-mode");
auto node_7 = GnuOption::Init("--debug");
auto node_8 = Argument::Init("<doc_name>");
auto node_9 = LogicAnd::Init();
node_9->AddChild(node_7);
node_9->AddChild(node_8);
auto node_10 = LogicXor::Init();
node_10->AddChild(node_5);
node_10->AddChild(node_6);
node_10->AddChild(node_9);
auto node_11 = Doc::Init();
node_11->AddChild(node_10);
cpp_codegen_info.doc_node_ = node_11;

return cpp_codegen_info;
}
CppCodegenInfo cpp_codegen_info = InitCppCodegenInfo();
}  // namespace clidoc
