#pragma once

#include "mono-core/logging/logger_config.hpp"

#include <type_traits>

namespace rflect::config {

/*
 * Base config for game builded with the rflect
 * engine
 */
struct Engine {
  config::Logger logger {};
};

template<typename T>
T example() {
  return {};
}

/*
 * All game configs must inherit from config::Rflect
 * and provide an example configuration
 */
template<typename T>
concept Game = requires(T t) {
  { example<T>() } -> std::same_as<T>;
  { t.engine } -> std::same_as<Engine&>;
};

} // namespace rflect::config
