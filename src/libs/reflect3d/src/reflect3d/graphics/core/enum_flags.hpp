#pragma once

#include <type_traits>
#include <utility>

namespace rf3d::detail {

template<typename T>
  requires(std::is_scoped_enum_v<T>)
consteval bool contains_flag(T const mask, T const flag) {
  return (std::to_underlying(mask) & std::to_underlying(flag)) != 0;
}

template<auto Mask, auto Flag>
concept ContainsFlag = detail::contains_flag(Mask, Flag);

} // namespace rf3d::detail
