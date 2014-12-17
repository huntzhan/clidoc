
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

// Forward declaration FlexGeneratedScanner to resolve cyclic #include.
namespace clidoc { class FlexGeneratedScanner; }
}

%code {
#include "generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex

#include <memory>
#include "tokenizer.h"
#include "utils.h"

using namespace clidoc::tokenizer;

// Error report function.
void clidoc::BisonGeneratedParser::error (const std::string&) { /* empty */ }
}

%parse-param { clidoc::FlexGeneratedScanner *lexer_ptr }

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

// Logical nodes.
%type <Doc::SharedPtr>               doc
%type <LogicAnd::SharedPtr>          utilities
%type <LogicAnd::SharedPtr>          seqs
%type <LogicAnd::SharedPtr>          descriptions
%type <LogicAnd::SharedPtr>          bindings
%type <LogicAnd::SharedPtr>          comments
%type <LogicXor::SharedPtr>          or_exprs
%type <SharedPtrNode>                single_seq
%type <SharedPtrNode>                atom

%type <UsageSection::SharedPtr>      usage_section
%type <SingleUtility::SharedPtr>     single_utility
%type <GnuOptionUnit::SharedPtr>     gnu_option_unit
%type <PosixOptionUnit::SharedPtr>   posix_option_unit

%type <OptionsSection::SharedPtr>    options_section
%type <SingleDescription::SharedPtr> single_description
%type <SingleBinding::SharedPtr>     single_binding
%type <DefaultValue::SharedPtr>      default_value
%type <SingleComment::SharedPtr>     single_comment

%%

doc : usage_section options_section {  }
;

usage_section : K_USAGE_COLON utilities {  }
;

utilities : utilities single_utility {  }
          | single_utility { }
;

single_utility : K_UTILITY_DELIMITER or_exprs {  }
;

or_exprs : or_exprs K_EXCLUSIVE_OR seqs {  }
         | seqs {  }
;

seqs : seqs single_seq {  }
     | single_seq {  }
;

single_seq : atom {  }
           | atom K_ELLIPSES {  }
;

atom : K_L_PARENTHESIS or_exprs K_R_PARENTHESIS { }
     | K_L_BRACKET or_exprs K_R_BRACKET         { }
     | posix_option_unit                    { }
     | gnu_option_unit                      { }
     | OPTION_ARGUEMENT                     { }
     | OPERAND                              { }
     | K_OPTIONS                            { }
;

posix_option_unit : POSIX_OPTION {  }
                  | GROUPED_OPTIONS {  }
;

gnu_option_unit : GNU_OPTION {
  // so verbose!!!! try to fix it.
  auto result = GnuOptionUnit::Init();
  auto gnu_option = T_GnuOption::Init(InitToken(TypeID::GNU_OPTION, $1));
  result->children_.push_back(gnu_option);
  $$ = result;
}
                | GNU_OPTION K_EQUAL_SIGN OPTION_ARGUEMENT {
}
;

options_section : K_OPTIONS_COLON descriptions {  }
;

descriptions : descriptions single_description {  }
             | single_description {  }
;

single_description : bindings default_value comments {  }
;

default_value : K_L_BRACKET K_DEFAULT_COLON OPTION_DEFAULT_VALUE K_R_BRACKET {  }
              | %empty {  }
;

comments : comments single_comment {  }
         | single_comment {  }
;

single_comment : COMMENT K_DESC_DELIMITER { }
               | K_DESC_DELIMITER { }
;

bindings : bindings single_binding {  }
         | single_binding {  }
;

single_binding : POSIX_OPTION {  }
               | GNU_OPTION {  }
               | POSIX_OPTION OPTION_ARGUEMENT {  }
               | GNU_OPTION OPTION_ARGUEMENT {  }
               | GNU_OPTION K_EQUAL_SIGN OPTION_ARGUEMENT {  }
; 

%%
