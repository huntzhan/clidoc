
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
#include "parser_utils.h"

// Forward declaration FlexGeneratedScanner to resolve cyclic #include.
namespace clidoc { class FlexGeneratedScanner; }

// TODO
class Undefine {};
}

%code {
#include "generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex

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
  L_PARENTHESIS       "("
  R_PARENTHESIS       ")"
  L_BRACKET           "["
  R_BRACKET           "]"
  EXCLUSIVE_OR        "|"
  ELLIPSES            "..."
  K_USAGE_COLON       "usage:"
  K_OPTIONS_COLON     "options:"
  K_DEFAULT_COLON     "default:"
  K_OPTIONS           "options"
  K_UTILITY_DELIMITER "*UTILITY_DELIMITER*"
  K_DESC_DELIMITER    "*DESC_DELIMITER*"

  // Only for argument processing.
  K_DOUBLE_HYPHEN     "--"

  // Shared.
  EQUAL_SIGN          "="
  
  // end-of-file.
  END                 0
;

%type <Undefine> doc

%type <Undefine> usage_section
%type <Undefine> utilities
%type <Undefine> single_utility
%type <Undefine> or_exprs
%type <Undefine> seqs
%type <Undefine> single_seq
%type <Undefine> atom
%type <Undefine> gnu_option_unit
%type <Undefine> posix_option_unit

%type <Undefine> options_section
%type <Undefine> descriptions
%type <Undefine> single_description
%type <Undefine> bindings
%type <Undefine> single_binding
%type <Undefine> default_value
%type <Undefine> comments
%type <Undefine> single_comment

%%

doc : usage_section options_section {  }
;

usage_section : K_USAGE_COLON utilities {  }
;

utilities : utilities single_utility {  }
          | single_utility
;

single_utility : K_UTILITY_DELIMITER or_exprs {  }
;

or_exprs : or_exprs EXCLUSIVE_OR seqs {  }
         | seqs {  }
;

seqs : seqs single_seq {  }
     | single_seq {  }
;

single_seq : atom {  }
           | atom ELLIPSES {  }
;

atom : L_PARENTHESIS or_exprs R_PARENTHESIS { }
     | L_BRACKET or_exprs R_BRACKET         { }
     | posix_option_unit                    { }
     | gnu_option_unit                      { }
     | OPTION_ARGUEMENT                     { }
     | OPERAND                              { }
     | K_OPTIONS                            { }
;

posix_option_unit : POSIX_OPTION {  }
                  | GROUPED_OPTIONS {  }
;

gnu_option_unit : GNU_OPTION {  }
                | GNU_OPTION EQUAL_SIGN OPTION_ARGUEMENT {  }
;

options_section : K_OPTIONS_COLON descriptions {  }
;

descriptions : descriptions single_description {  }
             | single_description {  }
;

single_description : bindings default_value comments {  }
;

default_value : L_BRACKET K_DEFAULT_COLON OPTION_DEFAULT_VALUE R_BRACKET {  }
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
               | GNU_OPTION EQUAL_SIGN OPTION_ARGUEMENT {  }
; 

%%
