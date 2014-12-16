#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "utils.h"
#include "tokenizer.h"

using namespace clidoc;
using std::string;
using std::vector;

vector<TerminalType> ExtractTokenType(const vector<Token> &tokens) {
  vector<TerminalType> types;
  for (const Token &token : tokens) {
    types.push_back(token.type());
  }
  return types;
}

void CheckTokenTypes(const string &text,
                     const vector<TerminalType> &types) {
  auto tokens = tokenizer::FromString(text);
  EXPECT_EQ(types, ExtractTokenType(tokens));
}

void CheckTokenValues(const string &text,
                      const vector<string> &values) {
  auto tokens = tokenizer::FromString(text);
  ASSERT_TRUE(tokens.size() == values.size());
  int index = 0;
  for (const auto &token : tokens) {
    // token must has value.
    EXPECT_TRUE(token.has_value());
    // token must has value.
    EXPECT_EQ(values[index], token.value());
    ++index;
  }
}
