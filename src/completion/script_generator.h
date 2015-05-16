#ifndef SRC_COMPLETION_SCRIPT_GENERATOR_H_
#define SRC_COMPLETION_SCRIPT_GENERATOR_H_

#include<string>
#include<fstream>

#include "ast_analyser.h"

using std::string;
using std::ofstream;

namespace clidoc{

class ScriptGenerator{
public:
  ScriptGenerator(const AstInfo& info);
  ScriptGenerator(
    const AstInfo& info, 
    const std::string& output_name);

  void GenerateScript();
  void InstallScript(std::string path);
  void InstallScript();
  static bool IsExist(const std::string& filename);

  ~ScriptGenerator() {
    if(!ofs_.is_open())
      ofs_.close();
  }

private:
  const AstInfo& ast_info_;
  std::ofstream ofs_;
  std::string output_name_;

  std::string ExtractScriptName(const std::string& path);
  void CopyScript(std::string path);
};

} //namespace clidoc

#endif //SRC_COMPLETION_SCRIPT_GENERATOR_H_
