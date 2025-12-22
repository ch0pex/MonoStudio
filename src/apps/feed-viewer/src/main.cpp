
#include "feed-viewer/program/config.hpp"
#include "feed-viewer/program/dispatcher.hpp"

#include <mono/config/config_tags.hpp>
#include <mono/config/parser.hpp>
#include <mono/execution/execution.hpp>
#include <mono/execution/expect.hpp>
#include <mono/program/handle_error.hpp>
#include <mono/program/options.hpp>


int main(int argc, char** argv) {
  namespace exec = mono::ex;

  auto run_program = exec::just(std::span(argv, argc)) //
                     | exec::expect(mono::program::parse_options<feed::ProgramConfig, mono::config::tag::Default>) //
                     | exec::inspect(mono::config::init_logger<feed::ProgramConfig>) //
                     | exec::then(feed::run_viewer) //
                     | exec::let_error(mono::program::handle_error);

  mono::ex::sync_wait(run_program);
}
