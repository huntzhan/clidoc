
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
      InitToken(TerminalType::POSIX_OPTION, "-o"),
      InitToken(TerminalType::ARGUMENT, "FILE"));
  recorder.ProcessCachedBindings();

  AmbiguityHandler visitor(&recorder);
  xor_1->Accept(&visitor);
  EXPECT_EQ("LogicXor(LogicOr(PosixOption[-o], Argument[FILE]))",
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

TEST(process_logic, FocusedElementCollector) {
  auto and_1 = LogicAnd::Init();
  and_1->AddChild(PosixOption::Init("-c"));
  and_1->AddChild(Argument::Init("ARG1"));
  and_1->AddChild(Argument::Init("ARG2"));
  and_1->AddChild(GnuOption::Init("--output"));
  and_1->AddChild(Argument::Init("FILE"));

  OptionBindingRecorder recorder;
  recorder.RecordBinding(
      InitToken(TerminalType::POSIX_OPTION, "-c"),
      InitToken(TerminalType::ARGUMENT, "ARG1"));
  recorder.RecordBinding(
      InitToken(TerminalType::GNU_OPTION, "--output"),
      InitToken(TerminalType::ARGUMENT, "FILE"));
  recorder.ProcessCachedBindings();

  FocusedElementCollector visitor(&recorder);
  and_1->Accept(&visitor);
  auto focused_elements = visitor.GetFocusedElement();

  EXPECT_EQ(unsigned(3), focused_elements.size());
  EXPECT_EQ("--output", focused_elements[0].value());
  EXPECT_EQ("-c", focused_elements[1].value());
  EXPECT_EQ("ARG2", focused_elements[2].value());
}
