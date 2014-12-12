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

DIGIT             [0-9]
LOWERCASE         [a-z]
UPPERCASE         [A-Z]
LOWERCASE_DIGIT   {LOWERCASE}|{DIGIT}
UPPERCASE_DIGIT   {UPPERCASE}|{DIGIT}
ALNUM             {LOWERCASE}|{UPPERCASE}|{DIGIT}

OPTION            -{ALNUM}
OPTION_ARGUEMENT  (<{LOWERCASE_DIGIT}+>)|({UPPERCASE_DIGIT}+)
GROUPED_OPTIONS   -{ALNUM}+
GNU_LONG_OPTION   --{ALNUM}+
ARGUMENT          {ALNUM}+

%%
{OPTION}            { clidoc::TokenHandler::AddToken(YYText(), "OPTION"); }
{OPTION_ARGUEMENT}  { clidoc::TokenHandler::AddToken(YYText(), "OPTION_ARGUEMENT"); }
{GROUPED_OPTIONS}   { clidoc::TokenHandler::AddToken(YYText(), "GROUPED_OPTIONS"); }
{GNU_LONG_OPTION}   { clidoc::TokenHandler::AddToken(YYText(), "GNU_LONG_OPTION"); }
{ARGUMENT}          { clidoc::TokenHandler::AddToken(YYText(), "ARGUMENT"); }
[,\|\[\]\n=]        { clidoc::TokenHandler::AddToken(YYText(), "STRUCTURE"); }
