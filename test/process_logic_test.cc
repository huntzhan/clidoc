
#include "gtest/gtest.h"
#include "ast/token_proxy.h"
#include "ast/parser_proxy.h"
#include "ast/process_logic.h"
#include "ast/ast_nodes.h"

using namespace clidoc;

TEST(process_logic, StructureOptimizer) {
  auto and_1 = LogicAnd::Init();
  auto and_2 = LogicAnd::Init();
  auto command_ptr = Command::Init("whatever");
  and_1->AddChild(and_2);
  and_2->AddChild(command_ptr);
  
  StructureOptimizer visitor;
  and_1->Accept(&visitor);

  EXPECT_EQ("LogicAnd(Command[whatever])", and_1->ToString());
}

TEST(process_logic, AmbiguityHandler) {
  auto xor_1 = LogicXor::Init();
  xor_1->AddChild(GroupedOptions::Init("-oFILE"));
  
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

TEST(process_logic, TerminalTypeModifier) {
  auto and_1 = LogicAnd::Init();
  and_1->AddChild(PosixOption::Init("-c"));
  and_1->AddChild(Command::Init(">whatever<"));
  and_1->AddChild(GnuOption::Init("--long"));

  TerminalTypeModifier<Argument> type_modifier;
  and_1->Accept(&type_modifier);

  EXPECT_EQ("LogicAnd("
            "Argument[-c], Argument[>whatever<], Argument[--long])",
            and_1->ToString());
}

TEST(process_logic, DoubleHyphenHandler) {
  auto and_1 = LogicAnd::Init();
  and_1->AddChild(PosixOption::Init("-c"));
  and_1->AddChild(KDoubleHyphen::Init("--"));
  and_1->AddChild(PosixOption::Init("-c"));
  and_1->AddChild(Command::Init(">whatever<"));
  and_1->AddChild(GnuOption::Init("--long"));

  DoubleHyphenHandler visitor;
  and_1->Accept(&visitor);
  EXPECT_EQ("LogicAnd(PosixOption[-c], "
            "Argument[-c], Argument[>whatever<], Argument[--long])",
            and_1->ToString());
}
