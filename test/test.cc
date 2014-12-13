
#include <vector>
#include "gtest/gtest.h"
#include "utils.h"
#include "tokenizer.h"
#include "generated_parser.h"

using std::vector;
using namespace clidoc;
using token_type = BisonGeneratedParser::token_type;
using ts = BisonGeneratedParser::token;

vector<token_type> ExtractTokenType(const vector<Token> &tokens) {
  vector<token_type> types;
  for (const Token &token : tokens) {
    types.push_back(token.type());
  }
  return types;
}

TEST(tokenizer, simple_case) {
  auto tokens = Tokenizer::FromString("-c --long <arg> ARG");
  vector<token_type> result = {
      ts::POSIX_OPTION, ts::GNU_OPTION, ts::ARGUEMENT, ts::ARGUEMENT};
  EXPECT_EQ(result, ExtractTokenType(tokens));
}
