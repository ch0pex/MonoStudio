
#include <reflect3d/graphics/core/renderpass_descriptor.hpp>
#include <reflect3d/graphics/core/resource_state.hpp>
#include <reflect3d/graphics/core/shader/compiler.hpp>
#include <reflect3d/graphics/core/shader/targets.hpp>
#include <reflect3d/graphics/rhi.hpp>
#include <reflect3d/input/input.hpp>
#include <reflect3d/window/window.hpp>
#include <reflect3d/window/window_builder.hpp>

#include <mono/execution/signals.hpp>
#include <mono/logging/logger.hpp>

#include "common.hpp"


int main() try {
  rf3d::rhi::VertexBuffer vertex_buffer {test::triangle::vertices};
  rf3d::rhi::IndexBuffer index_buffer {test::triangle::indices};
  rf3d::rhi::Surface surface {rf3d::WindowBuilder().default_callbacks().build()};
  auto pso = test::create_basic_pso();

  std::stop_token const token = mono::ex::setup_signals();
  while (not token.stop_requested()) {
    rf3d::input::poll_events();

    auto& frame_ctx   = rf3d::rhi::gpu::new_frame();
    auto* back_buffer = surface.next_image(frame_ctx.index);

    if (back_buffer == nullptr) {
      continue;
    }

    rf3d::rhi::DrawStreamBuilder draw_stream {};
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
                rf3d::ColorTarget {.texture = *back_buffer, .final_state = rf3d::ResourceState::present},
              },
          .draw_area = rf3d::DrawArea {
            .viewport   = surface.viewport(),
            .scissor    = surface.viewport().rect,
            .draw_calls = draw_stream.get_stream(),
          },
        }
    );

    frame_ctx.command_list.end();
    rf3d::rhi::gpu::submit_work(rf3d::rhi::default_submit_info(frame_ctx, surface));
    surface.present();
  }
  rf3d::rhi::gpu::wait_idle();
}
catch (std::exception const& e) {
  LOG_ERROR("Program execution terminated with an unhandled exception: {}", e.what());
  return EXIT_FAILURE;
}
catch (...) {
  LOG_ERROR("Program execution terminated with an unknown error");
  return EXIT_FAILURE;
}
