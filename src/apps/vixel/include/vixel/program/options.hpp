#pragma once

#include "vixel/program/config/config.hpp"

#include <mono/program/options.hpp>
#include <mono/config/parser.hpp>
#include <mono/error/expected.hpp>

namespace vix::po {

/**
 * Vixel may start without config argument
 */
inline mono::err::expected<config::Vixel> parse_options(std::span<char*> const args) {
  // if (args.size() <= 2)
  //   return config::Vixel {};
  return mono::program::parse_options<config::Vixel>(args);
}

} // namespace vix::po
