#pragma once

#include "vixel/program/config/config.hpp"

#include <mono-core/program/options.hpp>
#include <mono-core/config/parser.hpp>
#include <mono-core/error/expected.hpp>

namespace vix::po {

/**
 * Vixel may start without config argument
 */
inline rflect::err::expected<config::Vixel> parse_options(std::span<char*> const args) {
  // if (args.size() <= 2)
  //   return config::Vixel {};
  return rflect::po::parse_options<config::Vixel>(args);
}

} // namespace vix::po
