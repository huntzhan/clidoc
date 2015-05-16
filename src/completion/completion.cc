#include<iostream>
#include<fstream>
#include<iterator>

#include "completion.h"


using std::string;
using std::ifstream;
using std::istreambuf_iterator;
using std::cerr;
using std::endl;

namespace clidoc {

void ParseScriptArguments(
    const string &doc_path,
    const string &output_file_name,
    bool is_manual) {
  if(!clidoc::ScriptGenerator::IsExist(doc_path)) {
    throw("doc not found");
    return ;
  }

  ifstream fin(doc_path);
  clidoc::CodeGenInfo info;
  const string doc(
      (istreambuf_iterator<char>(fin)),
      istreambuf_iterator<char>());
  fin.close();
  info.Prepare(doc);
  clidoc::AstAnalyser analyser(info,doc_path);
  clidoc::ScriptGenerator generator(
                                    analyser.GetAstInfo(),
                                    output_file_name);
  generator.GenerateScript();
  if(!is_manual){
    generator.InstallScript();
  }
  return ;
}

}
