#ifndef SRC_COMPLETION_AST_ANALYSER_H_
#define SRC_COMPLETION_AST_ANALYSER_H_

#include <string>

#include "clidoc/ast/ast_build.h"

using std::string;

namespace clidoc{

struct AstInfo{
  std::string words_list_;
  std::string util_name_;
};

class AstAnalyser{
public:
  AstAnalyser(const CodeGenInfo& info,const string& doc_path);
  AstInfo GetAstInfo();
  
  std::string GetUtilityName(std::string path);
  std::string ExtractUtilityName(const std::string& doc_text);
  std::string ExtractWordsList(const CodeGenInfo& info);

private:
  AstInfo ast_info_;
};

} //namespace clidoc
#endif  //SRC_COMPLETION_AST_ANALYSER_H_
