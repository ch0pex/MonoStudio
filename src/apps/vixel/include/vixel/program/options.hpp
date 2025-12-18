#pragma once

#include "mono/logging/logger.hpp"
#include "vixel/program/config/config.hpp"

#include <mono/config/parser.hpp>
#include <mono/error/expected.hpp>
#include <mono/program/options.hpp>
#include <stdexcept>

namespace vix::po {

/**
 * Vixel may start without config argument
 */
inline config::Vixel parse_options(std::span<char*> const args) {
  if (args.size() <= 2) {
    LOG_WARNING("No arguments were provided using default Vixel configuration instead");
    return config::Vixel {};
  }
  return mono::program::parse_options<config::Vixel>(args).value();
}

} // namespace vix::po
