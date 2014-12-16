#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "utils.h"
#include "tokenizer.h"

using namespace clidoc;
using namespace clidoc::tokenizer;
using std::string;
using std::vector;
using std::ofstream;
using std::istringstream;

vector<TerminalType> ExtractTokenType(const vector<Token> &tokens) {
  vector<TerminalType> types;
  for (const Token &token : tokens) {
    types.push_back(token.type());
  }
  return types;
}

void CheckTokenTypes(const string &text,
                     const vector<TerminalType> &types) {
  auto tokens = FromString(text);
  EXPECT_EQ(types, ExtractTokenType(tokens));
}

void CheckTokenValues(const string &text,
                      const vector<string> &values) {
  auto tokens = FromString(text);
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

void CheckRawTokenTypes(const string &text,
                        const vector<Type> types) {
  ofstream null_ostream("/dev/null");
  istringstream input_stream(text);
  FlexGeneratedScanner lexer(&input_stream, &null_ostream);
  int index = 0;
  while (true) {
    auto item = lexer.lex();
    auto type_id = item.token();
    if (type_id == TypeID::END) {
      // finish.
      break;
    }
    EXPECT_EQ(types[index++], type_id);
  }
  ASSERT_EQ(types.size(), index);
}

void CheckRawTokenValues(const string &text,
                         const vector<string> values) {
  ofstream null_ostream("/dev/null");
  istringstream input_stream(text);
  FlexGeneratedScanner lexer(&input_stream, &null_ostream);
  int index = 0;
  while (true) {
    auto item = lexer.lex();
    auto type_id = item.token();
    auto value = item.value.as<string>();
    if (type_id == TypeID::END) {
      // finish.
      break;
    }
    EXPECT_EQ(values[index++], value);
  }
  ASSERT_EQ(values.size(), index);
}
