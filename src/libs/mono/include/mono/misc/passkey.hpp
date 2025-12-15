#pragma once

namespace mono {

/**
 * PassKey class can be used as key for those restricted constructors
 * this is very handy when you want to restrict the creating of an object
 */
template<typename T>
class PassKey {
  friend T;
  explicit PassKey()      = default;
  PassKey(PassKey const&) = default;
  ~PassKey()              = default;

public:
  PassKey(PassKey&&)                 = delete;
  PassKey& operator=(PassKey const&) = delete;
  PassKey& operator=(PassKey&&)      = delete;
};

} // namespace mono
