
#include "vixel/program/config/config.hpp"
#include "vixel/program/dispatcher.hpp"
#include "vixel/program/options.hpp"

#include <mono/config/parser.hpp>
#include <mono/error/log_error.hpp>

int main() {
  using namespace vix;
  //
  // auto result = vix::po::parse_options(mono::span(argv, argc)) //
  //                                                             // .and_then(mono::config::init_logger<config::Vixel>)
  //                   .and_then(program::run)
  //                   .or_else(mono::err::log<config::Vixel>);

  // return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
