#include <cstddef>

#include "gtest/gtest.h"

#include "ast/ast_nodes.h"
#include "shared/smart_ptr_interface.h"

using namespace clidoc;
using std::size_t;

TEST(utils, smart_ptr_count) {
  {
    auto ptr_1 = LogicAnd::Init();
    auto ptr_2 = LogicXor::Init();
    auto ptr_3 = LogicXor::Init();
  }
  EXPECT_EQ(size_t(3), SPIStaticDataMember::cache_container_.size());
}


TEST(utils, smart_ptr_free) {
  auto ptr = LogicAnd::Init();
  EXPECT_EQ(long(2), ptr.use_count());
  SPIStaticDataMember::FreeCache();
  EXPECT_EQ(long(1), ptr.use_count());
}
