
#include "feed-viewer/feed-viewer.hpp"
#include "mono/config/parser.hpp"
#include "mono/execution/inspect.hpp"

#include <mono/program/handle_error.hpp>

#include <rfl/toml/load.hpp>


int main(int argc, char** argv) {
  namespace exec = mono::ex;

  // auto run_program = exec::just(std::span(argv, argc)) //
  // | exec::then() //
  // | exec::tap(mono::config::init_logger) //
  // | exec::then(feed::run_viewer) //
  // | exec::let_error(mono::program::handle_error);

  // mono::ex::sync_wait(run_program);
}
