
#include "gtest/gtest.h"
#include "smart_ptr_interface.h"
#include "utils.h"

using namespace clidoc;

TEST(utils, smart_ptr_count) {
  {
    auto ptr_1 = LogicAnd::Init();
    auto ptr_2 = LogicXor::Init();
    auto ptr_3 = Command::Init(Token());
  }
  EXPECT_EQ(3, SPIStaticDataMember::cached_container_.size());
}


TEST(utils, smart_ptr_free) {
  auto ptr = LogicAnd::Init();
  EXPECT_EQ(2, ptr.use_count());
  SPIStaticDataMember::FreeCache();
  EXPECT_EQ(1, ptr.use_count());
}
