
#include <string>
#include "gtest/gtest.h"
#include "ast/ast_nodes.h"
#include "ast/ast_node_interface.h"

using namespace clidoc;
using std::string;

TEST(symbol, terminal_to_string) {
  auto Command_ptr = Command::Init("whatever");
  EXPECT_EQ("Command[whatever]", Command_ptr->ToString());
}

TEST(symbol, non_terminal_to_string) {
  // no child.
  EXPECT_EQ("LogicAnd()", LogicAnd::Init()->ToString());
  // one child.
  auto Command_ptr = Command::Init("whatever");
  auto logic_and_ptr = LogicAnd::Init();
  logic_and_ptr->children_.push_back(Command_ptr);
  EXPECT_EQ("LogicAnd(Command[whatever])", logic_and_ptr->ToString());
  // more than one.
  auto posix_option_ptr = PosixOption::Init("-c");
  logic_and_ptr->children_.push_back(posix_option_ptr);
  EXPECT_EQ(
      "LogicAnd(Command[whatever], PosixOption[-c])",
      logic_and_ptr->ToString());
  // more than one level.
  auto logic_xor_ptr = LogicXor::Init();
  auto gnu_option_ptr = GnuOption::Init("--long");
  logic_xor_ptr->children_.push_back(gnu_option_ptr);
  logic_and_ptr->children_.push_back(logic_xor_ptr);
  EXPECT_EQ(
      ("LogicAnd(Command[whatever], PosixOption[-c], "
         "LogicXor(GnuOption[--long])"
       ")"),
      logic_and_ptr->ToString());
}

TEST(TokenTest, ToString) {
  auto t = Token(TerminalType::POSIX_OPTION, "-c");
  EXPECT_EQ("Token(TerminalType::POSIX_OPTION, \"-c\")", t.ToString());
}
