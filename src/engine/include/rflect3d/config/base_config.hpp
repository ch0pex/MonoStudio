#pragma once

#include "rflect3d/core/logging/logger_config.hpp"

#include <type_traits>

namespace rflect::config {

/*
 * Base config for game builded with the rflect
 * engine
 */
struct Base {
  config::Logger logger {};
};

template<typename T>
  requires(std::is_base_of_v<Base, T>)
T example() {
  return {};
}

/*
 * All game configs must inherit from config::Rflect
 * and provide an example configuration
 */
template<typename T>
concept Game = std::is_base_of_v<Base, T> and requires(T t) {
  { example<T>() } -> std::same_as<T>;
};

} // namespace rflect::config
