
#include "vixel/program/config/config.hpp"
#include "vixel/program/dispatcher.hpp"
#include "vixel/program/options.hpp"

#include <rflect3d/config/parser.hpp>
#include <rflect3d/core/error/log_error.hpp>

int main(int argc, char** argv) {
  using namespace vix;

  auto result = vix::po::parse_options(std::span(argv, argc)) //
                    .and_then(rflect::config::init_logger<config::Vixel>)
                    .and_then(program::run)
                    .or_else(rflect::err::log<config::Vixel>);

  return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
