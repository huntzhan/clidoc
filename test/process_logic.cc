
#include "gtest/gtest.h"
#include "process_logic.h"
#include "utils.h"

using namespace clidoc;

auto t_operand = Token(TerminalType::OPERAND, "whatever");

TEST(process_logic, simple_case) {
  auto and_1 = LogicAnd::Init();
  auto and_2 = LogicAnd::Init();
  auto operand_ptr = Operand::Init(t_operand);
  and_1->children_.push_back(and_2);
  and_2->children_.push_back(operand_ptr);
  
  StructureOptimizer visitor;
  and_1->Accept(&visitor);

  EXPECT_EQ("LogicAnd(Operand[whatever])", and_1->ToString());
}
