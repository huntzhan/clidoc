#include "clidoc/ast/smart_ptr_interface.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace clidoc {

// definition of static data member.
vector<shared_ptr<void>> SPIStaticDataMember::cache_container_;

void SPIStaticDataMember::FreeCache() {
  cache_container_.clear();
}

}  // namespace clidoc
