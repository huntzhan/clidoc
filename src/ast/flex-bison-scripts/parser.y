
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

#include <stdexcept>
#include <memory>

#include "ast/parser_proxy.h"
#include "ast/generated_scanner.h"
#include "ast/ast_nodes.h"
#include "ast/option_record.h"

#undef yylex
#define yylex lexer_ptr->lex

// Error report function.
void clidoc::BisonGeneratedParser::error(const std::string &msg) {
  throw std::logic_error("clidoc::BisonGeneratedParser::error");
}

// functions that faciliate node building.
template <typename WeakNonTerminalType>
void NonTerminalAddChild(WeakNonTerminalType parent_node) {
  /* end of recursion. */
}

template <typename WeakNonTerminalType,
          typename WeakNodeType, typename... OtherWeakNodeTypes>
void NonTerminalAddChild(
    WeakNonTerminalType parent_node,
    std::weak_ptr<WeakNodeType> child_node,
    std::weak_ptr<OtherWeakNodeTypes>... other_nodes) {
  // child_node is std::weak_ptr<...>.
  parent_node.lock()->AddChild(child_node.lock());
  NonTerminalAddChild(parent_node, other_nodes...);
}

template <typename WeakNonTerminalType,
          typename NodeType, typename... OtherNodeTypes>
void NonTerminalAddChild(
    WeakNonTerminalType parent_node,
    std::shared_ptr<NodeType> child_node,
    std::shared_ptr<OtherNodeTypes>... other_nodes) {
  // child_node is std::shared_ptr<...>.
  parent_node.lock()->AddChild(child_node);
  NonTerminalAddChild(parent_node, other_nodes...);
}

template <typename NonTerminalType, typename... WeakNodeTypes>
typename NonTerminalType::SharedPtr
BuildNonTerminal(WeakNodeTypes... child_nodes) {
  auto parent_node = NonTerminalType::Init();
  typename NonTerminalType::WeakPtr weak_parent_node = parent_node;
  NonTerminalAddChild(weak_parent_node, child_nodes...);
  return parent_node;
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
%type <LogicXor::WeakPtr>               xor_exprs

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
  auto doc = BuildNonTerminal<Doc>($1);
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
  NonTerminalAddChild($1, $2);
  $$ = $1;
}
          | single_utility {
  $$ = BuildNonTerminal<LogicXor>($1);
}
;


// single_utility : K_UTILITY_DELIMITER xor_exprs
// ;
single_utility : K_UTILITY_DELIMITER xor_exprs {
  $$ = $2;
}
;


// xor_exprs : xor_exprs K_EXCLUSIVE_OR seqs
//           | seqs
// ;
xor_exprs : xor_exprs K_EXCLUSIVE_OR seqs {
  NonTerminalAddChild($1, $3);
  $$ = $1;
}
         | seqs {
  $$ = BuildNonTerminal<LogicXor>($1);
}
;


// seqs : seqs single_seq
//      | single_seq
// ;
seqs : seqs single_seq {
  NonTerminalAddChild($1, $2);
  $$ = $1;
}
     | single_seq {
  $$ = BuildNonTerminal<LogicAnd>($1);
}
;


// single_seq : atom
//            | atom K_ELLIPSES
// ;
single_seq : atom {
  $$ = $1;
}
           | atom K_ELLIPSES {
  $$ = BuildNonTerminal<LogicOneOrMore>($1);
}
;


// atom : K_L_PARENTHESIS xor_exprs K_R_PARENTHESIS
//      | K_L_BRACKET xor_exprs K_R_BRACKET
//      | posix_option_unit
//      | gnu_option_unit
//      | ARGUMENT
//      | COMMAND
//      | K_OPTIONS
//      | K_DOUBLE_HYPHEN
// ;
atom : K_L_PARENTHESIS xor_exprs K_R_PARENTHESIS {
  $$ = BuildNonTerminal<LogicAnd>($2);
}
     | K_L_BRACKET xor_exprs K_R_BRACKET {
  $$ = BuildNonTerminal<LogicOptional>($2);
}
     | posix_option_unit {
  $$ = $1;
}
     | gnu_option_unit {
  $$ = $1;
}
     | ARGUMENT {
  $$ = BuildNonTerminal<LogicAnd>(Argument::Init($1));
}
     | COMMAND {
  $$ = BuildNonTerminal<LogicAnd>(Command::Init($1));
}
     | K_OPTIONS {
  $$ = BuildNonTerminal<LogicAnd>(KOptions::Init());
}
     | K_DOUBLE_HYPHEN {
  $$ = BuildNonTerminal<LogicAnd>(KDoubleHyphen::Init());
}
;


// posix_option_unit : POSIX_OPTION
//                   | GROUPED_OPTIONS
// ;
posix_option_unit : POSIX_OPTION {
  $$ = BuildNonTerminal<LogicAnd>(PosixOption::Init($1));
}
                  | GROUPED_OPTIONS {
  $$ = BuildNonTerminal<LogicAnd>(GroupedOptions::Init($1));
}
;


// gnu_option_unit : GNU_OPTION
//                 | GNU_OPTION K_EQUAL_SIGN ARGUMENT
// ;
gnu_option_unit : GNU_OPTION {
  $$ = BuildNonTerminal<LogicAnd>(GnuOption::Init($1));
}
                | GNU_OPTION K_EQUAL_SIGN ARGUMENT {
  // recording binding.
  recorder_ptr->RecordBinding(
      Token(TerminalType::GNU_OPTION, $1),
      Token(TerminalType::ARGUMENT, $3));

  // normal works.
  $$ = BuildNonTerminal<LogicAnd>(GnuOption::Init($1),
                                  Argument::Init($3));
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
  $$ = DefaultValue::Init($3);
}
              | %empty {
  $$ = DefaultValue::Init();
}
;


// bindings : bindings single_binding {  }
//          | single_binding {  }
// ;
bindings : bindings single_binding {
  NonTerminalAddChild($1, $2);
  $$ = $1;

}
         | single_binding {
  $$ = BuildNonTerminal<OptionBindingContainer>($1);
}
;


// single_binding : POSIX_OPTION
//                | GNU_OPTION
//                | POSIX_OPTION ARGUMENT
//                | GNU_OPTION ARGUMENT
//                | GNU_OPTION K_EQUAL_SIGN ARGUMENT
// ;
single_binding : POSIX_OPTION {
  $$ = OptionBinding::Init(Token(TerminalType::POSIX_OPTION, $1));
}
               | GNU_OPTION {
  $$ = OptionBinding::Init(Token(TerminalType::GNU_OPTION, $1));
}
               | POSIX_OPTION ARGUMENT {
  $$ = OptionBinding::Init(Token(TerminalType::POSIX_OPTION, $1),
                           Token(TerminalType::ARGUMENT, $2));
}
               | GNU_OPTION ARGUMENT {
  $$ = OptionBinding::Init(Token(TerminalType::GNU_OPTION, $1),
                                 Token(TerminalType::ARGUMENT, $2));
}
               | GNU_OPTION K_EQUAL_SIGN ARGUMENT {
  $$ = OptionBinding::Init(Token(TerminalType::GNU_OPTION, $1),
                                 Token(TerminalType::ARGUMENT, $3));
}
;

%%
