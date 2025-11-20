#pragma once

namespace rflect {

/**
 * PassKey class can be used as key for those restricted constructors
 * this is very handy when you want to restrict the creating of an object
 */
template<typename T>
class PassKey {
  friend T;
  explicit PassKey()      = default;
  PassKey(PassKey const&) = default;
};

} // namespace rflect
