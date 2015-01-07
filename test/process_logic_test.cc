
#include "gtest/gtest.h"
#include "ast/token_proxy.h"
#include "ast/parser_proxy.h"
#include "ast/process_logic.h"
#include "ast/ast_nodes.h"

using namespace clidoc;

auto t_command = Token(TerminalType::COMMAND, "whatever");
auto t_grouped_options = Token(TerminalType::GROUPED_OPTIONS, "-oFILE");

TEST(process_logic, StructureOptimizer) {
  auto and_1 = LogicAnd::Init();
  auto and_2 = LogicAnd::Init();
  auto command_ptr = Command::Init(t_command);
  and_1->AddChild(and_2);
  and_2->AddChild(command_ptr);
  
  StructureOptimizer visitor;
  and_1->Accept(&visitor);

  EXPECT_EQ("LogicAnd(Command[whatever])", and_1->ToString());
}

TEST(process_logic, AmbiguityHandler) {
  auto xor_1 = LogicXor::Init();
  xor_1->AddChild(GroupedOptions::Init(t_grouped_options));
  
  OptionBindingRecorder recorder;
  recorder.RecordBinding(
      InitToken(TypeID::POSIX_OPTION, "-o"),
      InitToken(TypeID::ARGUMENT, "FILE"));
  recorder.ProcessCachedBindings();

  AmbiguityHandler visitor(&recorder);
  xor_1->Accept(&visitor);
  EXPECT_EQ("LogicXor(LogicAnd(PosixOption[-o], Argument[FILE]))",
            xor_1->ToString());
}
