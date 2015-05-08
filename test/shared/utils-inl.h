#include <sstream>
#include <cstddef>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "clidoc/ast/ast_node_interface.h"
#include "clidoc/ast/ast_nodes.h"
#include "clidoc/tokenizer.h"

using namespace clidoc;

using std::size_t;
using std::string;
using std::vector;
using std::istringstream;
using std::ostringstream;

// facilitate usage.
using Type = clidoc::BisonGeneratedParser::token_type;
using TypeID = clidoc::BisonGeneratedParser::token;

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
    EXPECT_EQ(values[index++], token.value());
  }
}

void CheckRawTokenTypes(const string &text,
                        const vector<Type> types) {
  ostringstream null_ostream;
  istringstream input_stream(text);
  FlexGeneratedScannerClass lexer(&input_stream, &null_ostream);
  size_t index = 0;
  while (true) {
    auto item = lexer.lex();
    auto type_id = item.token();
    if (type_id == TypeID::END) {
      // finish.
      break;
    }
    ASSERT_TRUE(index < types.size());
    EXPECT_EQ(types[index++], type_id);
  }
  ASSERT_EQ(types.size(), index);
}

void CheckRawTokenValues(const string &text,
                         const vector<string> values) {
  ostringstream null_ostream;
  istringstream input_stream(text);
  FlexGeneratedScannerClass lexer(&input_stream, &null_ostream);
  size_t index = 0;
  while (true) {
    auto item = lexer.lex();
    auto type_id = item.token();
    const string &value = item.value.as<string>();
    if (type_id == TypeID::END) {
      // finish.
      break;
    }
    ASSERT_TRUE(index < values.size());
    EXPECT_EQ(values[index++], value);
  }
  ASSERT_EQ(values.size(), index);
}
