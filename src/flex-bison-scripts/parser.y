
%skeleton "lalr1.cc"
%require "3.0.2"

%defines     "generated_parser.h"
%output      "generated_parser.cc"
%name-prefix "clidoc"

%define parser_class_name {BisonGeneratedParser}
%define api.token.constructor
%define api.value.type variant

%code requires {
#include <string>
#include "utils.h"

namespace clidoc {
// Forward declaration `FlexGeneratedScanner` to resolve cyclic #include.
class FlexGeneratedScanner;
}  // namespace clidoc

}

%code {
#include "generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex

#include <memory>
// TODO:
// #include "parser_proxy.h"
#include "tokenizer.h"
#include "utils.h"

using namespace clidoc::tokenizer;

// Error report function.
void clidoc::BisonGeneratedParser::error (const std::string&) { /* empty */ }
}

%parse-param { clidoc::FlexGeneratedScanner *lexer_ptr }
%parse-param { clidoc::Doc::SharedPtr *doc_ptr }
// %parse-param { clidoc::OptionsBindingRecorder *option_binding_recorder_ptr }

// Terminal with value.
%token <std::string>
  // Only for doc processing.
  OPTION_ARGUEMENT
  OPERAND
  OPTION_DEFAULT_VALUE
  COMMENT

  // Only for argument processing.
  ARGUMENT

  // Shared.
  POSIX_OPTION
  GROUPED_OPTIONS
  GNU_OPTION
;

// Terminal without value.
%token
  // Only for doc processing.
  K_L_PARENTHESIS     "("
  K_R_PARENTHESIS     ")"
  K_L_BRACKET         "["
  K_R_BRACKET         "]"
  K_EXCLUSIVE_OR      "|"
  K_ELLIPSES          "..."
  K_USAGE_COLON       "usage:"
  K_OPTIONS_COLON     "options:"
  K_DEFAULT_COLON     "default:"
  K_OPTIONS           "options"
  K_UTILITY_DELIMITER "*UTILITY_DELIMITER*"
  K_DESC_DELIMITER    "*DESC_DELIMITER*"

  // Only for argument processing.
  K_DOUBLE_HYPHEN     "--"

  // Shared.
  K_EQUAL_SIGN        "="

  // end-of-file.
  END                 0
;

// Logical                  nodes.
%type <Doc::SharedPtr>      doc
%type <LogicAnd::SharedPtr> seqs
%type <LogicAnd::SharedPtr> descriptions
%type <LogicAnd::SharedPtr> comments
%type <LogicXor::SharedPtr> utilities
%type <LogicXor::SharedPtr> or_exprs

%type <SharedPtrNode>       usage_section
%type <SharedPtrNode>       single_seq
%type <SharedPtrNode>       atom
%type <SharedPtrNode>       single_utility
%type <SharedPtrNode>       gnu_option_unit
%type <SharedPtrNode>       posix_option_unit

%type <SharedPtrNode>                     options_section
%type <SharedPtrNode>                     single_description
%type <OptionBindingContainer::SharedPtr> bindings
%type <OptionBinding::SharedPtr>          single_binding
%type <DefaultValue::SharedPtr>           default_value
%type <SharedPtrNode>                     single_comment

%%


// doc : usage_section options_section
// ;
doc : usage_section options_section {
  auto doc = Doc::Init();
  doc->children_.push_back($1);
  *doc_ptr = doc;
  $$ = doc;
}
;


// usage_section : K_USAGE_COLON utilities
// ;
usage_section : K_USAGE_COLON utilities {
  $$ = $2;
}
;


// utilities : utilities single_utility
//           | single_utility
// ;
utilities : utilities single_utility {
  $1->children_.push_back($2);
  $$ = $1;
}
          | single_utility {
  auto logic_xor = LogicXor::Init();
  logic_xor->children_.push_back($1);
  $$ = logic_xor;
}
;


// single_utility : K_UTILITY_DELIMITER or_exprs
// ;
single_utility : K_UTILITY_DELIMITER or_exprs {
  $$ = $2;
}
;


// or_exprs : or_exprs K_EXCLUSIVE_OR seqs
//          | seqs
// ;
or_exprs : or_exprs K_EXCLUSIVE_OR seqs {
  $1->children_.push_back($3);
  $$ = $1;
}
         | seqs {
  auto logic_xor = LogicXor::Init();
  logic_xor->children_.push_back($1);
  $$ = logic_xor;
}
;


// seqs : seqs single_seq
//      | single_seq
// ;
seqs : seqs single_seq {
  $1->children_.push_back($2);
  $$ = $1;
}
     | single_seq {
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back($1);
  $$ = logic_and;
}
;


// single_seq : atom
//            | atom K_ELLIPSES
// ;
single_seq : atom {
  $$ = $1;
}
           | atom K_ELLIPSES {
  auto logic_one_or_more = LogicOneOrMore::Init();
  logic_one_or_more->children_.push_back($1);
  $$ = logic_one_or_more;
}
;


// atom : K_L_PARENTHESIS or_exprs K_R_PARENTHESIS
//      | K_L_BRACKET or_exprs K_R_BRACKET
//      | posix_option_unit
//      | gnu_option_unit
//      | OPTION_ARGUEMENT
//      | OPERAND
//      | K_OPTIONS
// ;
atom : K_L_PARENTHESIS or_exprs K_R_PARENTHESIS {
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back($2);
  $$ = logic_and;
}
     | K_L_BRACKET or_exprs K_R_BRACKET {
  auto logic_optional = LogicOptional::Init();
  logic_optional->children_.push_back($2);
  $$ = logic_optional;
}
     | posix_option_unit {
  $$ = $1;
}
     | gnu_option_unit {
  $$ = $1;
}
     | OPTION_ARGUEMENT {
  auto option_argument =
      OptionArguement::Init(InitToken(TypeID::OPTION_ARGUEMENT, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(option_argument);
  $$ = logic_and;
}
     | OPERAND {
  auto operand =
      Operand::Init(InitToken(TypeID::OPERAND, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(operand);
  $$ = logic_and;
}
     | K_OPTIONS {
  auto k_options =
      KOptions::Init(InitToken(TypeID::K_OPTIONS, ""));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(k_options);
  $$ = logic_and;
}
;


// posix_option_unit : POSIX_OPTION
//                   | GROUPED_OPTIONS
// ;
posix_option_unit : POSIX_OPTION {
  auto posix_option =
      PosixOption::Init(InitToken(TypeID::POSIX_OPTION, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(posix_option);
  $$ = logic_and;
}
                  | GROUPED_OPTIONS {
  auto grouped_option =
      GroupedOptions::Init(InitToken(TypeID::GROUPED_OPTIONS, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(grouped_option);
  $$ = logic_and;
}
;


// gnu_option_unit : GNU_OPTION
//                 | GNU_OPTION K_EQUAL_SIGN OPTION_ARGUEMENT
// ;
gnu_option_unit : GNU_OPTION {
  auto gnu_option = GnuOption::Init(InitToken(TypeID::GNU_OPTION, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(gnu_option);
  $$ = logic_and;
}
                | GNU_OPTION K_EQUAL_SIGN OPTION_ARGUEMENT {
  // The driver must crate binding for option and option_argument.
  auto gnu_option = GnuOption::Init(InitToken(TypeID::GNU_OPTION, $1));
  auto option_argument =
      OptionArguement::Init(InitToken(TypeID::OPTION_ARGUEMENT, $3));
  auto logic_and = LogicAnd::Init();
  logic_and->children_.push_back(gnu_option);
  logic_and->children_.push_back(option_argument);
  $$ = logic_and;
}
;


// follow

options_section : K_OPTIONS_COLON descriptions {  }
;


descriptions : descriptions single_description {  }
             | single_description {  }
;


// single_description : bindings default_value comments {  }
// ;
single_description : bindings default_value comments {
  // record bindings and default value.
}
;


default_value : K_L_BRACKET K_DEFAULT_COLON OPTION_DEFAULT_VALUE K_R_BRACKET {  
  auto default_value =
      DefaultValue::Init(InitToken(TypeID::OPTION_DEFAULT_VALUE, $3));
  $$ = default_value;
}
              | %empty {
  $$ = DefaultValue::Init();
}
;


comments : comments single_comment {  }
         | single_comment {  }
;


single_comment : COMMENT K_DESC_DELIMITER { }
               | K_DESC_DELIMITER { }
;


// bindings : bindings single_binding {  }
//          | single_binding {  }
// ;
bindings : bindings single_binding {
  $1->children_.push_back($2);
  $$ = $1;

}
         | single_binding {
  auto bindings = OptionBindingContainer::Init();
  bindings->children_.push_back($1);
  $$ = bindings;
}
;


// single_binding : POSIX_OPTION
//                | GNU_OPTION
//                | POSIX_OPTION OPTION_ARGUEMENT
//                | GNU_OPTION OPTION_ARGUEMENT
//                | GNU_OPTION K_EQUAL_SIGN OPTION_ARGUEMENT
// ;
single_binding : POSIX_OPTION {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::POSIX_OPTION, $1));
  $$ = binding;
}
               | GNU_OPTION {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, $1));
  $$ = binding;
}
               | POSIX_OPTION OPTION_ARGUEMENT {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::POSIX_OPTION, $1),
                          InitToken(TypeID::OPTION_ARGUEMENT, $2));
  $$ = binding;
}
               | GNU_OPTION OPTION_ARGUEMENT {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, $1),
                          InitToken(TypeID::OPTION_ARGUEMENT, $2));
  $$ = binding;
}
               | GNU_OPTION K_EQUAL_SIGN OPTION_ARGUEMENT {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, $1),
                          InitToken(TypeID::OPTION_ARGUEMENT, $3));
  $$ = binding;
}
;

%%
