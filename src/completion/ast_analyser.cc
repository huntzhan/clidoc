#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <iterator>

#include "clidoc/ast/ast_node_interface.h"
#include "ast_analyser.h"
#include "script_generator.h"
#include "compl_string_util.h"

using std::cout;
using std::endl;
using std::string;
using std::cerr;
using std::ifstream;
using std::istreambuf_iterator;

namespace clidoc{

AstAnalyser::AstAnalyser(
    const CodeGenInfo& info, 
    const string& doc_path) {
  this->ast_info_.util_name_ = GetUtilityName(doc_path);
  this->ast_info_.words_list_ = ExtractWordsList(info);
}

AstInfo AstAnalyser::GetAstInfo(){
  return this->ast_info_;
}

std::string AstAnalyser::GetUtilityName(std::string doc_path) {
  if(!clidoc::ScriptGenerator::IsExist(doc_path)) {
    throw("doc not found");
    return "";
  }
  ifstream fin(doc_path);
  string doc(
      (istreambuf_iterator<char>(fin)),
      istreambuf_iterator<char>());
  fin.close();
  return ExtractUtilityName(doc);
}

std::string AstAnalyser::ExtractUtilityName(
    const std::string& doc_text) {
  std::string str = doc_text;
  ComplStringUtil::RemoveHeadCommentsAndBlank(str);
  if(str=="") {
    throw("cannot find utility name");
    return ""; 
  }
  str = str.substr(6);
  ComplStringUtil::RemoveHeadCommentsAndBlank(str);
  if(str=="") {
    throw("cannot find utility name");
    return ""; 
  }
  str = ComplStringUtil::ExtractTheFirstString(str);
  return str;
}

std::string AstAnalyser::ExtractWordsList(const CodeGenInfo& info){
  std::string words_list="";
  for(Token element:info.bound_options_) {
    words_list+=element.value()+" ";
  }

  for(Token element:info.unbound_options_) {
    words_list+=element.value()+" ";
  }

  for(Token element:info.oom_bound_options_) {
     words_list+=element.value()+" ";
  }

  for(Token element:info.commands_) {
     words_list+=element.value()+" ";
  }

  ComplStringUtil::TrimString(words_list);
  return words_list;
}
 
} //namespace clidoc
