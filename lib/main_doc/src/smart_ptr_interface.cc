
#include <memory>
#include <vector>
#include "clidoc/ast/smart_ptr_interface.h"

using std::vector;
using std::shared_ptr;

namespace clidoc {

// definition of static data member.
vector<shared_ptr<void>> SPIStaticDataMember::cache_container_;

void SPIStaticDataMember::FreeCache() {
  cache_container_.clear();
}

}  // namespace clidoc
