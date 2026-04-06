
#include <reflect3d/graphics/core2/renderpass_descriptor.hpp>
#include <reflect3d/graphics/core2/resource_state.hpp>
#include <reflect3d/graphics/vk2/buffer.hpp>
#include <reflect3d/graphics/vk2/draw_stream.hpp>
#include <reflect3d/graphics/vk2/gpu.hpp>
#include <reflect3d/graphics/vk2/surface.hpp>
#include <reflect3d/input/input.hpp>
#include <reflect3d/window/window.hpp>
#include <reflect3d/window/window_builder.hpp>

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>
#include "mono/containers/static_vector.hpp"

int main() try {

  mono::static_vector<rf3d::vk::Surface, 3> surfaces {};

  // create surfaces
  surfaces.reserve(3);
  for (std::uint8_t i = 0; i < 3; ++i) {
    surfaces.emplace_back(rf3d::WindowBuilder().default_callbacks().build());
  }

  std::stop_token token = mono::ex::setup_signals();
  while (not token.stop_requested()) {

    for (auto& surface: surfaces) {
      rf3d::input::poll_events();

      auto& frame_ctx = rf3d::vk::Gpu::new_frame();
      auto* image     = surface.next_image(frame_ctx.index);

      if (image == nullptr) {
        continue;
      }

      rf3d::vk::Gpu::submit_frame(frame_ctx, surface);
      surface.present();
    }
  }
}
catch (std::exception const& e) {
  LOG_ERROR("Program execution terminated with an unhandled exception: {}", e.what());
  return EXIT_FAILURE;
}
catch (...) {
  LOG_ERROR("Program execution terminated with an unknown error");
  return EXIT_FAILURE;
}
