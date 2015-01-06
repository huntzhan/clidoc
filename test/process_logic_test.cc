
#include "gtest/gtest.h"
#include "ast/process_logic.h"
#include "ast/ast_nodes.h"

using namespace clidoc;

auto t_command = Token(TerminalType::COMMAND, "whatever");

TEST(process_logic, simple_case) {
  auto and_1 = LogicAnd::Init();
  auto and_2 = LogicAnd::Init();
  auto command_ptr = Command::Init(t_command);
  and_1->children_.push_back(and_2);
  and_2->children_.push_back(command_ptr);
  
  StructureOptimizer visitor;
  and_1->Accept(&visitor);

  EXPECT_EQ("LogicAnd(Command[whatever])", and_1->ToString());
}
