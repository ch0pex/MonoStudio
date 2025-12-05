#pragma once

#include "rflect3d/core/error/expected.hpp"
#include "rflect3d/program/options.hpp"
#include "vixel/program/config/config.hpp"

namespace vix::po {

/**
 * Vixel may start without config argument
 */
inline rflect::err::expected<config::Vixel> parse_options(std::span<char*> const args) {
  if (args.size() <= 2) {
    LOG_INFO("No config was provided using, default game configuration");
    return config::Vixel {};
  }
  return rflect::po::parse_options<config::Vixel>(args);
}

} // namespace vix::po
