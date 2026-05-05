
#include "common.hpp"

#include <reflect3d/graphics/core/renderpass_descriptor.hpp>
#include <reflect3d/graphics/core/resource_state.hpp>
#include <reflect3d/graphics/core/shader/compiler.hpp>
#include <reflect3d/graphics/core/shader/targets.hpp>
#include <reflect3d/graphics/rhi.hpp>
#include <reflect3d/graphics/vk/gpu.hpp>
#include <reflect3d/graphics/vk/pso.hpp>
#include <reflect3d/input/input.hpp>
#include <reflect3d/window/window.hpp>
#include <reflect3d/window/window_builder.hpp>

#include <assets_path.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>


template<rf3d::RenderHardwareInterface Impl>
void test_square() {
  rf3d::rhi::vertex_buffer_t<Impl> vertex_buffer {test::square::vertices};
  rf3d::rhi::index_buffer_t<Impl> index_buffer {test::square::indices};

  auto pso = test::create_basic_pso<Impl>();

  rf3d::rhi::surface_t<Impl> surface {rf3d::WindowBuilder().default_callbacks().build()};
  std::stop_token const token = mono::ex::setup_signals();

  while (not token.stop_requested()) {
    rf3d::input::poll_events();

    auto& frame_ctx = rf3d::rhi::gpu_t<Impl>::new_frame();
    auto* image     = surface.next_image();

    if (image == nullptr) {
      continue;
    }

    rf3d::rhi::draw_stream_builder_t<Impl> draw_stream {};
    draw_stream.draw({
      .pso           = pso,
      .vertex_buffer = vertex_buffer,
      .index_buffer  = index_buffer,
      .draw_params   = {
          .index_count = static_cast<std::uint32_t>(index_buffer.size()),
      },
    });

    frame_ctx.command_list.render_pass(
        rf3d::RenderPass {
          .debug_name = "Test Render Pass",
          .render_targets =
              rf3d::RenderTargets {
                rf3d::ColorTarget {.texture = *image, .final_state = rf3d::ResourceState::present},
              },
          .draw_area = rf3d::DrawArea {
            .viewport   = surface.viewport(),
            .scissor    = surface.viewport().rect,
            .draw_calls = draw_stream.get_stream(),
          },
        }
    );

    frame_ctx.command_list.end();
    rf3d::rhi::gpu_t<Impl>::submit_work(rf3d::rhi::default_submit_info(frame_ctx, surface));
    surface.present();
  }
}

int main() try { //
  test_square<rf3d::impl::vk>();
}
catch (std::exception const& e) {
  LOG_ERROR("Program execution terminated with an unhandled exception: {}", e.what());
  return EXIT_FAILURE;
}
catch (...) {
  LOG_ERROR("Program execution terminated with an unknown error");
  return EXIT_FAILURE;
}
