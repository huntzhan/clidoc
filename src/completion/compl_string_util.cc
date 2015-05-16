#include<string>

#include "compl_string_util.h"

namespace clidoc{

void ComplStringUtil::RemoveHeadCommentsAndBlank(string& str) {
  unsigned pos = 0;
  for(;pos<str.size();pos++) {
    for(;pos<str.size()&&str[pos]==' ';pos++);
    if(pos>=str.size())
      break;
    else if(str[pos]=='\n') {
      continue;
    }
    else if(str[pos]=='#') {
      for(;pos<str.size()&&str[pos]!='\n';pos++);
      if(pos>=str.size())
        break;
    }
    else break;
  }
  if(pos>=str.size()){
    str = "";
    return ;
  }
  str = str.substr(pos);
  return ;
}

string ComplStringUtil::ExtractTheFirstString(const string& str) {
  unsigned int begin=0;
  for(begin=0;begin<str.size();begin++) {
    if(str[begin]!=' '&&str[begin]!='\n')
    break;
  }
  unsigned int length=begin;
  for(;length<str.size();length++) {
    if(str[length]==' '||str[length]=='\n')
      break;
  }
  length = length - begin;
  return str.substr(begin,length);
}

void ComplStringUtil::TrimString(string& str) {
  if (str.empty()) {  
    return ;  
  }  

  str.erase(0,str.find_first_not_of(" \n"));  
  str.erase(str.find_last_not_of(" \n") + 1);  
  return ;
}

}// namespace clidoc
