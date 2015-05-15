#ifndef SRC_COMPLETION_COMPL_STRING_UTIL_H_
#define SRC_COMPLETION_COMPL_STRING_UTIL_H_

#include<string>

using std::string;

namespace clidoc {

class ComplStringUtil {
public:
static void RemoveHeadCommentsAndBlank(string& str);
static string ExtractTheFirstString(const string& str);
static void TrimString(string& str);

};

} //namespace clidoc
#endif //SRC_COMPLETION_COMPL_STRING_UTIL_H_
