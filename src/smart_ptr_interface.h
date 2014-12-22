#ifndef SRC_SMART_PTR_INTERFACE_H_
#define SRC_SMART_PTR_INTERFACE_H_

#include <memory>
#include <utility>
#include <vector>

namespace clidoc {

// SPI stands for SmartPtrInterface.
struct SPIStaticDataMember {
  // manually free cache.
  static void FreeCached();
  // static data member can not be placed in class template in this case.
  static std::vector<std::shared_ptr<void>> cached_container_;
};

template <typename Derived>
class SmartPtrInterface
    // enable `this->shared_from_this()`.
    : public std::enable_shared_from_this<Derived> {
 public:
  // type alias.
  using SharedPtr = std::shared_ptr<Derived>;
  using WeakPtr = std::weak_ptr<Derived>;
  // function for init shared_ptr.
  template <typename... Args>
  static SharedPtr Init(Args&&... args);
};

template <typename Derived>
template <typename... Args>
typename SmartPtrInterface<Derived>::SharedPtr
    SmartPtrInterface<Derived>::Init(Args&&... args) {
  // Since the bison generated parser use placement new to create an instance
  // during parsing, and hence shared_ptr should not be used as non-terminal
  // types. As an intuitive approach, I tried to use weak_ptr as non-terminal
  // types, but in what approach could I store the corrensponding shared_ptr?
  // Luckly, the shared_ptr has a feature called "type ensure":
  // http://stackoverflow.com/questions/3899790/shared-ptr-magic
  // Hence, Init(...) generated shared_ptr would place an copy to the
  // `SPIStaticDataMember::cached_container_`, the value_type of which is
  // std::shared_ptr<void>.
  auto ptr = std::make_shared<Derived>(std::forward<Args>(args)...);
  SPIStaticDataMember::cached_container_.push_back(ptr);
  return ptr;
}

}  // namespace clidoc

#endif  // SRC_SMART_PTR_INTERFACE_H_
