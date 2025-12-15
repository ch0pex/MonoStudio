#pragma once

#include <mono/error/expected.hpp>
#include <mono/execution/execution.hpp>
#include <mono/execution/signals.hpp>
#include <mono/program/handle_error.hpp>
#include <reflect3d/input/input.hpp>

#include "vixel/program/config/config.hpp"
#include "vixel/program/main_window.hpp"

namespace vix::program {

constexpr auto run = [](config::Vixel&& config) -> mono::err::expected<config::Vixel> {
  namespace exec = mono::ex;
  exec::static_thread_pool main_pool {std::thread::hardware_concurrency() >= 4U ? 3U : 1U};
  auto sched = main_pool.get_scheduler();

  exec::setup_signals();

  auto window = main_window();
  while (exec::should_run()) {
    rf3d::input::poll_events();

    auto frame_work = //
        exec::when_all(
            exec::starts_on(sched, exec::just(/* deltatime, gamestate */) | exec::then([]() { })), // Game
            exec::starts_on(sched, exec::just(/* previous gamesate    */) | exec::then([]() { })), // Render
            exec::starts_on(sched, exec::just(/* execution commands   */) | exec::then([]() { })) // GPU
        ) //
        | exec::let_error(mono::program::handle_error);

    exec::sync_wait(frame_work);
  }

  return {std::move(config)};
};

} // namespace vix::program
