
%skeleton "lalr1.cc"
%require "3.0.2"

%defines     "generated_parser.h"
%output      "generated_parser.cc"
%name-prefix "clidoc"

%define parser_class_name {BisonGeneratedParser}
%define api.token.constructor
%define api.value.type variant

// code inserted in header.
%code requires {

#include <string>
#include "ast/ast_nodes.h"
#include "ast/option_record.h"

// Forward declarations.
namespace clidoc {

class FlexGeneratedScanner;

}  // namespace clidoc

}  // %code requires

// code inserted in implementation.
%code {

#include <iostream>

#include "ast/parser_proxy.h"
#include "ast/generated_scanner.h"
#undef yylex
#define yylex lexer_ptr->lex

#include "ast/ast_nodes.h"
#include "ast/option_record.h"
#include "ast/token_proxy.h"


// Error report function.
void clidoc::BisonGeneratedParser::error(const std::string &msg) {
  std::cerr << "clidoc::BisonGeneratedParser::error" << std::endl
            << msg << std::endl;
}

}  // %code

%parse-param { clidoc::FlexGeneratedScanner *lexer_ptr }
%parse-param { clidoc::Doc::SharedPtr *doc_node_ptr }
%parse-param { clidoc::OptionBindingRecorder *recorder_ptr }

// Terminal with value.
%token <std::string>
  // Shared.
  POSIX_OPTION
  GROUPED_OPTIONS
  GNU_OPTION

  // Only for doc processing.
  ARGUMENT
  DEFAULT_VALUE
  COMMAND

  // Only for argument processing.
  GENERAL_ELEMENT
;

// Terminal without value.
%token
  // Only for doc processing.
  K_L_PARENTHESIS     
  K_R_PARENTHESIS     
  K_L_BRACKET         
  K_R_BRACKET         
  K_EXCLUSIVE_OR      
  K_ELLIPSES          
  K_USAGE_COLON       
  K_OPTIONS_COLON     
  K_DEFAULT_COLON     
  K_OPTIONS           
  K_UTILITY_DELIMITER 
  K_DESC_DELIMITER    

  // Shared.
  K_DOUBLE_HYPHEN     
  K_EQUAL_SIGN        

  // end-of-file.
  END                 0
;

// Logical nodes.
%type <Doc::WeakPtr>                    doc
%type <LogicAnd::WeakPtr>               seqs
%type <LogicAnd::WeakPtr>               descriptions
%type <LogicXor::WeakPtr>               utilities
%type <LogicXor::WeakPtr>               or_exprs

%type <WeakPtrNode>                     usage_section
%type <WeakPtrNode>                     single_seq
%type <WeakPtrNode>                     atom
%type <WeakPtrNode>                     single_utility
%type <WeakPtrNode>                     gnu_option_unit
%type <WeakPtrNode>                     posix_option_unit

%type <WeakPtrNode>                     options_section
%type <WeakPtrNode>                     single_description
%type <OptionBindingContainer::WeakPtr> bindings
%type <OptionBinding::WeakPtr>          single_binding
%type <DefaultValue::WeakPtr>           default_value

%%


// doc : usage_section options_section
// ;
doc : usage_section options_section {
  auto doc = Doc::Init();
  doc->AddChild($1.lock());
  *doc_node_ptr = doc;
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
  $1.lock()->AddChild($2.lock());
  $$ = $1;
}
          | single_utility {
  auto logic_xor = LogicXor::Init();
  logic_xor->AddChild($1.lock());
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
  $1.lock()->AddChild($3.lock());
  $$ = $1;
}
         | seqs {
  auto logic_xor = LogicXor::Init();
  logic_xor->AddChild($1.lock());
  $$ = logic_xor;
}
;


// seqs : seqs single_seq
//      | single_seq
// ;
seqs : seqs single_seq {
  $1.lock()->AddChild($2.lock());
  $$ = $1;
}
     | single_seq {
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild($1.lock());
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
  logic_one_or_more->AddChild($1.lock());
  $$ = logic_one_or_more;
}
;


// atom : K_L_PARENTHESIS or_exprs K_R_PARENTHESIS
//      | K_L_BRACKET or_exprs K_R_BRACKET
//      | posix_option_unit
//      | gnu_option_unit
//      | ARGUMENT
//      | COMMAND
//      | K_OPTIONS
// ;
atom : K_L_PARENTHESIS or_exprs K_R_PARENTHESIS {
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild($2.lock());
  $$ = logic_and;
}
     | K_L_BRACKET or_exprs K_R_BRACKET {
  auto logic_optional = LogicOptional::Init();
  logic_optional->AddChild($2.lock());
  $$ = logic_optional;
}
     | posix_option_unit {
  $$ = $1;
}
     | gnu_option_unit {
  $$ = $1;
}
     | ARGUMENT {
  auto argument =
      Argument::Init(InitToken(TypeID::ARGUMENT, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(argument);
  $$ = logic_and;
}
     | COMMAND {
  auto command =
      Command::Init(InitToken(TypeID::COMMAND, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(command);
  $$ = logic_and;
}
     | K_OPTIONS {
  auto k_options =
      KOptions::Init(InitToken(TypeID::K_OPTIONS, ""));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(k_options);
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
  logic_and->AddChild(posix_option);
  $$ = logic_and;
}
                  | GROUPED_OPTIONS {
  auto grouped_option =
      GroupedOptions::Init(InitToken(TypeID::GROUPED_OPTIONS, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(grouped_option);
  $$ = logic_and;
}
;


// gnu_option_unit : GNU_OPTION
//                 | GNU_OPTION K_EQUAL_SIGN ARGUMENT
// ;
gnu_option_unit : GNU_OPTION {
  auto gnu_option = GnuOption::Init(InitToken(TypeID::GNU_OPTION, $1));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(gnu_option);
  $$ = logic_and;
}
                | GNU_OPTION K_EQUAL_SIGN ARGUMENT {
  // recording binding.
  recorder_ptr->RecordBinding(
      InitToken(TypeID::GNU_OPTION, $1),
      InitToken(TypeID::ARGUMENT, $3));

  // normal works.
  auto gnu_option = GnuOption::Init(InitToken(TypeID::GNU_OPTION, $1));
  auto argument =
      Argument::Init(InitToken(TypeID::ARGUMENT, $3));
  auto logic_and = LogicAnd::Init();
  logic_and->AddChild(gnu_option);
  logic_and->AddChild(argument);
  $$ = logic_and;
}
;


options_section : K_OPTIONS_COLON descriptions {  }
                | %empty { }
;


descriptions : descriptions single_description {  }
             | single_description {  }
;


// single_description : bindings default_value K_DESC_DELIMITER {  }
// ;
single_description : bindings default_value K_DESC_DELIMITER {
  recorder_ptr->RecordBinding($1.lock(), $2.lock());
}
;


default_value : K_L_BRACKET K_DEFAULT_COLON DEFAULT_VALUE K_R_BRACKET {  
  auto default_value =
      DefaultValue::Init(InitToken(TypeID::DEFAULT_VALUE, $3));
  $$ = default_value;
}
              | %empty {
  $$ = DefaultValue::Init();
}
;


// bindings : bindings single_binding {  }
//          | single_binding {  }
// ;
bindings : bindings single_binding {
  $1.lock()->AddChild($2.lock());
  $$ = $1;

}
         | single_binding {
  auto bindings = OptionBindingContainer::Init();
  bindings->AddChild($1.lock());
  $$ = bindings;
}
;


// single_binding : POSIX_OPTION
//                | GNU_OPTION
//                | POSIX_OPTION ARGUMENT
//                | GNU_OPTION ARGUMENT
//                | GNU_OPTION K_EQUAL_SIGN ARGUMENT
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
               | POSIX_OPTION ARGUMENT {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::POSIX_OPTION, $1),
                          InitToken(TypeID::ARGUMENT, $2));
  $$ = binding;
}
               | GNU_OPTION ARGUMENT {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, $1),
                          InitToken(TypeID::ARGUMENT, $2));
  $$ = binding;
}
               | GNU_OPTION K_EQUAL_SIGN ARGUMENT {
  auto binding =
      OptionBinding::Init(InitToken(TypeID::GNU_OPTION, $1),
                          InitToken(TypeID::ARGUMENT, $3));
  $$ = binding;
}
;

%%
