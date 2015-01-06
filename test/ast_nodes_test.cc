
#include <string>
#include "gtest/gtest.h"
#include "ast/ast_nodes.h"

using namespace clidoc;
using std::string;

auto t_command = Token(TerminalType::COMMAND, "whatever");
auto t_posix_option = Token(TerminalType::POSIX_OPTION, "-c");
auto t_gnu_option = Token(TerminalType::GNU_OPTION, "--long");

TEST(symbol, terminal_to_string) {
  auto Command_ptr = Command::Init(t_command);
  EXPECT_EQ("Command[whatever]", Command_ptr->ToString());
}

TEST(symbol, non_terminal_to_string) {
  // no child.
  EXPECT_EQ("LogicAnd()", LogicAnd::Init()->ToString());
  // one child.
  auto Command_ptr = Command::Init(t_command);
  auto logic_and_ptr = LogicAnd::Init();
  logic_and_ptr->children_.push_back(Command_ptr);
  EXPECT_EQ("LogicAnd(Command[whatever])", logic_and_ptr->ToString());
  // more than one.
  auto posix_option_ptr = PosixOption::Init(t_posix_option);
  logic_and_ptr->children_.push_back(posix_option_ptr);
  EXPECT_EQ(
      "LogicAnd(Command[whatever], PosixOption[-c])",
      logic_and_ptr->ToString());
  // more than one level.
  auto logic_xor_ptr = LogicXor::Init();
  auto gnu_option_ptr = GnuOption::Init(t_gnu_option);
  logic_xor_ptr->children_.push_back(gnu_option_ptr);
  logic_and_ptr->children_.push_back(logic_xor_ptr);
  EXPECT_EQ(
      ("LogicAnd(Command[whatever], PosixOption[-c], "
         "LogicXor(GnuOption[--long])"
       ")"),
      logic_and_ptr->ToString());
}
