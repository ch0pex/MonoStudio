#pragma once

#include <concepts>
#include <type_traits>

namespace mono::config {

struct Tag { };

template<typename T>
  requires(std::same_as<typename T::config_concept, Tag>)
T example() {
  return {};
}

/*
 * All game configs must inherit from config::Rflect
 * and provide an example configuration
 */
template<typename T>
concept Program = std::same_as<typename T::config_concept, Tag> and requires(T t) {
  { example<T>() } -> std::same_as<T>;
};

} // namespace mono::config
