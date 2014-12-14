#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "utils.h"
#include "tokenizer.h"

using namespace clidoc;
using std::string;
using std::vector;

vector<Token::Type> ExtractTokenType(const vector<Token> &tokens) {
  vector<Token::Type> type_ids;
  for (const Token &token : tokens) {
    type_ids.push_back(token.type_id());
  }
  return type_ids;
}

void CheckTokenTypes(const string &text,
                     const vector<Token::Type> &type_ids) {
  auto tokens = Tokenizer::FromString(text);
  EXPECT_EQ(type_ids, ExtractTokenType(tokens));
}

void CheckTokenValues(const string &text,
                      const vector<string> &values) {
  auto tokens = Tokenizer::FromString(text);
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
