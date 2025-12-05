#pragma once

#include <GLFW/glfw3.h>
#include <rflect3d/core/error/expected.hpp>
#include <rflect3d/core/execution/execution.hpp>
#include <rflect3d/core/execution/signals.hpp>
#include <rflect3d/input/input.hpp>
#include <rflect3d/program/handle_error.hpp>

#include "rflect3d/graphics/vk/vulkan.hpp"
#include "rflect3d/render/renderer.hpp"
#include "vixel/program/config/config.hpp"
#include "vixel/program/main_window.hpp"

namespace vix::program {

constexpr auto run = [](config::Vixel&& config) -> rflect::err::expected<config::Vixel> {
  namespace exec = rflect::ex;
  exec::static_thread_pool main_pool {std::thread::hardware_concurrency() >= 4U ? 3U : 1U};
  auto sched = main_pool.get_scheduler();

  exec::setup_signals();
  auto window   = main_window(config.game.window);
  auto renderer = rflect::Renderer<rflect::gfx::Vulkan> {};

  while (exec::should_run()) {
    rflect::input::poll_events();

    auto frame_work = // clang-format off
        exec::when_all(
            exec::starts_on(sched, exec::just(/* deltatime, gamestate */) | exec::then([]() { })), // Game
            exec::starts_on(sched, exec::just(/* previous gamesate    */) | exec::then([&]() { renderer.render(); })), // Render
            exec::starts_on(sched, exec::just(/* execution commands   */) | exec::then([]() { })) // GPU
        ) // clang-format on
        | exec::let_error(rflect::program::handle_error);

    exec::sync_wait(frame_work);
    window.swap_buffers();
  }

  return {std::move(config)};
};

} // namespace vix::program
