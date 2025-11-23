#pragma once

#include "rflect3d/config/parser.hpp"
#include "rflect3d/core/error/expected.hpp"
#include "rflect3d/program/options.hpp"
#include "vixel/program/config/config.hpp"

namespace vix::po {

/**
 * Vixel may start without config argument
 */
inline rflect::err::expected<config::Vixel> parse_options(std::span<char*> const args) {
  if (args.size() <= 2)
    return config::Vixel {};
  return rflect::po::parse_options<config::Vixel>(args);
}

} // namespace vix::po
