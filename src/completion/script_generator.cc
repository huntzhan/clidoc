#include<string>
#include<iostream>
#include<fstream>

#include "script_generator.h"


using std::string;
using std::ifstream;
using std::ofstream;

namespace clidoc{

ScriptGenerator::ScriptGenerator(
    const AstInfo& info):ast_info_(info) {
  this->output_name_ = info.util_name_+"_script.sh";	
}

ScriptGenerator::ScriptGenerator(
    const AstInfo& info, 
    const std::string& output_name):ast_info_(info) {
  if(output_name!="")
    this->output_name_ = output_name;
  else
    this->output_name_ = info.util_name_+"_script.sh";
}


void ScriptGenerator::GenerateScript()
{
  if(this->ofs_.is_open())
    ofs_.close();
  this->ofs_.open(this->output_name_, std::ofstream::out);
  ofs_<<
  "_"+this->ast_info_.util_name_+"(){\n"
  "  local cur\n"
  "  cur=\"${COMP_WORDS[COMP_CWORD]}\"\n"
  "\n"
  "  if [ ${COMP_CWORD} -ge 1 ]; then\n"
  "    COMPREPLY=( $( compgen -W \'"+
                      this->ast_info_.words_list_+
                      "\' -- ${cur}) )\n"
	
  "  fi\n"
  "}\n"
  "complete -F _"+
  this->ast_info_.util_name_+" "+this->ast_info_.util_name_;
  ofs_.close();
  return ;
}

void ScriptGenerator::InstallScript(std::string path) {
  if(path[path.size()-1]!='/')
    path+='/';
  CopyScript(path);
}

void ScriptGenerator::InstallScript() {
  std::string path = "/etc/bash_completion.d/";
  CopyScript(path);
}

void ScriptGenerator::CopyScript(std::string path) {
  std::string target = path+ExtractScriptName(this->output_name_);
  std::string source = this->output_name_;
  if(!IsExist(source)){
    throw("Source script not found!");
  }
  if(IsExist(target)) {	
    throw("Target script has existed!");
  }
  std::ifstream fin(source,ifstream::binary|ifstream::in);
  std::ofstream fout(target, ofstream::binary|ofstream::out);
  fout<<fin.rdbuf();
  fout.close();
  fin.close();
}

bool ScriptGenerator::IsExist(
    const std::string& filename) {
  ifstream ifs(filename);
  if(!ifs)
    return false;
  else{
    ifs.close();
    return true;
  }
}

std::string ScriptGenerator::ExtractScriptName(
    const std::string& path) {
  string str = path;
  str.erase(str.find_last_not_of(" \n") + 1);
  int tmp = str.find_last_of("/");
  if(tmp<0)
    return str;
  else{
    return str.substr(tmp+1);
  }
}

} //namespace clidoc

