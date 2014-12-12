
%skeleton "lalr1.cc"
%require "3.0.2"

%defines "generated_parser.h"
%output "generated_parser.cc"

%define parser_class_name {BisonGeneratedParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
#include <string>

class Undefine {};

// Forward declaration FlexGeneratedScanner to resolve cyclic #include.
class FlexGeneratedScanner;
}

%code {
#include "generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex
}

%parse-param { FlexGeneratedScanner *lexer_ptr }

%token <std::string>
  POSIX_OPTION GROUPED_OPTIONS GNU_OPTION
  ARGUEMENT OPERAND
	OPTION_DEFAULT_VALUE COMMENT
;

%token
  L_PARENTHESIS       "("
  R_PARENTHESIS       ")"
  L_BRACKET           "["
  R_BRACKET           "]"
  EXCLUSIVE_OR        "|"
  EQUAL_SIGN          "="
  ELLIPSES            "..."
  K_USAGE_COLON       "usage:"
  K_OPTIONS_COLON     "options:"
  K_DEFAULT_COLON     "default:"
  K_OPTIONS           "options"
  K_UTILITY_DELIMITER "*UTILITY_DELIMITER*"
  K_DESC_DELIMITER    "*DESC_DELIMITER*"
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
%type <Undefine> comment

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
     | ARGUEMENT                            { }
     | OPERAND                              { }
     | K_OPTIONS                            { }
;

posix_option_unit : POSIX_OPTION {  }
                  | GROUPED_OPTIONS {  }
;

gnu_option_unit : GNU_OPTION {  }
                | GNU_OPTION EQUAL_SIGN ARGUEMENT {  }
;

options_section : K_OPTIONS_COLON descriptions {  }
;

descriptions : descriptions single_description {  }
						 | single_description {  }
;

single_description : bindings default_value comment K_DESC_DELIMITER  {  }
;

default_value : L_BRACKET K_DEFAULT_COLON OPTION_DEFAULT_VALUE R_BRACKET {  }
              | %empty {  }
;

comment : COMMENT {  }
				| %empty {  }
;

bindings : bindings single_binding {  }
				 | single_binding {  }
;

single_binding : POSIX_OPTION {  }
               | GNU_OPTION {  }
               | POSIX_OPTION ARGUEMENT {  }
               | GNU_OPTION ARGUEMENT {  }
               | GNU_OPTION EQUAL_SIGN ARGUEMENT {  }
; 

%%
