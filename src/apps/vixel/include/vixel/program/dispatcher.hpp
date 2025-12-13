#pragma once

#include <mono-core/error/expected.hpp>
#include <mono-core/execution/execution.hpp>
#include <mono-core/execution/signals.hpp>
#include <mono-core/program/handle_error.hpp>
#include <reflect3d/input/input.hpp>

#include "vixel/program/config/config.hpp"
#include "vixel/program/main_window.hpp"

namespace vix::program {

constexpr auto run = [](config::Vixel&& config) -> rflect::err::expected<config::Vixel> {
  namespace exec = rflect::ex;
  exec::static_thread_pool main_pool {std::thread::hardware_concurrency() >= 4U ? 3U : 1U};
  auto sched = main_pool.get_scheduler();

  exec::setup_signals();

  auto window = main_window();
  while (exec::should_run()) {
    rflect::input::poll_events();

    auto frame_work = //
        exec::when_all(
            exec::starts_on(sched, exec::just(/* deltatime, gamestate */) | exec::then([]() { })), // Game
            exec::starts_on(sched, exec::just(/* previous gamesate    */) | exec::then([]() { })), // Render
            exec::starts_on(sched, exec::just(/* execution commands   */) | exec::then([]() { })) // GPU
        ) //
        | exec::let_error(rflect::program::handle_error);

    exec::sync_wait(frame_work);
  }

  return {std::move(config)};
};

} // namespace vix::program
