%{
#include <string>
#include <vector>

namespace clidoc {
// using declarations.
using std::string;
using std::vector;

class Token {
 public:
  Token(const string &lexeme, const string &syntactic_category)
      : lexeme_(lexeme), syntactic_category_(syntactic_category) {}
  string lexeme_;
  string syntactic_category_;
};

class TokenHandler {
 public:
  static void AddToken(const string &lexeme, const string &syntactic_category);
	static vector<Token> tokens;
};

void TokenHandler::AddToken(const string &lexeme,
                            const string &syntactic_category) {
  tokens.push_back(Token(lexeme, syntactic_category));
}

vector<Token> TokenHandler::tokens;  // init static member.

}  // namespace clidoc
%}

%option c++
%option noyywrap

ALNUM            [a-zA-Z0-9]
ARGUMENT         {ALNUM}+
OPTION           -{ALNUM}
GROUPED_OPTIONS  -{ALNUM}+
GNU_LONG_OPTION  --{ALNUM}+

%%
{ARGUMENT}         { clidoc::TokenHandler::AddToken(YYText(), "ARGUMENT"); }
{OPTION}           { clidoc::TokenHandler::AddToken(YYText(), "OPTION"); }
{GROUPED_OPTIONS}  { clidoc::TokenHandler::AddToken(YYText(), "GROUPED_OPTIONS"); }
{GNU_LONG_OPTION}  { clidoc::TokenHandler::AddToken(YYText(), "GNU_LONG_OPTION"); }
