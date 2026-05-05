
#include <reflect3d/graphics/rhi.hpp>
#include <reflect3d/input/input.hpp>
#include <reflect3d/window/window.hpp>
#include <reflect3d/window/window_builder.hpp>

#include <mono/containers/static_vector.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>

template<rf3d::RenderHardwareInterface Backend>
void test_surfaces() {
  using namespace rf3d::rhi;
  using surface_type = rf3d::rhi::surface_t<Backend>;

  constexpr std::size_t surface_count = 3;
  mono::static_vector<surface_type, surface_count> surfaces {};

  for (std::uint8_t i = 0; i < surface_count; ++i) {
    surfaces.emplace_back(rf3d::WindowBuilder().default_callbacks().build());
  }

  std::stop_token const token = mono::ex::setup_signals();
  while (not token.stop_requested()) {
    rf3d::input::poll_events();

    auto& frame_ctx = Backend::gpu::new_frame();

    mono::static_vector<surface_type*, surface_count> active {};
    for (auto& surface: surfaces) {
      if (auto* bb = surface.next_image(); bb != nullptr) {
        frame_ctx.command_list.barrier(*bb, rf3d::ResourceState::present);
        active.emplace_back(std::addressof(surface));
      }
    }
    Backend::gpu::submit_frame(frame_ctx, active);
    Backend::gpu::present(active);
  }
}

int main() try { //
  test_surfaces<rf3d::impl::vk>();
}
catch (std::exception const& e) {
  LOG_ERROR("Program execution terminated with an unhandled exception: {}", e.what());
  return EXIT_FAILURE;
}
catch (...) {
  LOG_ERROR("Program execution terminated with an unknown error");
  return EXIT_FAILURE;
}
