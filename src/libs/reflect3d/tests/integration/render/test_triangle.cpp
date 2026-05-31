
#include <reflect3d/graphics/core/renderpass_descriptor.hpp>
#include <reflect3d/graphics/core/resource_state.hpp>
#include <reflect3d/graphics/core/shader/compiler.hpp>
#include <reflect3d/graphics/core/shader/targets.hpp>
#include <reflect3d/graphics/rhi.hpp>
#include <reflect3d/input/input.hpp>
#include <reflect3d/window/window.hpp>
#include <reflect3d/window/window_builder.hpp>

#include <iostream>
#include <mono/execution/signals.hpp>
#include <mono/logging/logger.hpp>
#include "common.hpp"


int main() try {
  using namespace rf3d;

  auto index_buffer  = rhi::make_buffer<rhi::IndexBuffer>(test::triangle::indices);
  auto vertex_buffer = rhi::make_buffer<rhi::VertexBuffer>(test::triangle::vertices);
  rhi::Surface surface {rf3d::WindowBuilder().default_callbacks().build()};
  auto pso = test::create_basic_pso();

  std::stop_token const token = mono::ex::setup_signals();
  while (not token.stop_requested()) {
    input::poll_events();

    auto& frame_ctx   = rhi::gpu::new_frame();
    auto* back_buffer = surface.next_image();

    if (back_buffer == nullptr) {
      continue;
    }

    rhi::DrawStreamBuilder draw_stream {};
    draw_stream.draw({
      .pso           = pso,
      .vertex_buffer = vertex_buffer,
      .index_buffer  = index_buffer,
      .draw_params   = {
          .index_count = static_cast<std::uint32_t>(test::triangle::indices.size()),
      },
    });

    frame_ctx.command_list.render_pass(
        RenderPass {
          .debug_name = "Test Render Pass",
          .render_targets =
              RenderTargets {
                ColorTarget {.texture = *back_buffer, .final_state = ResourceState::present},
              },
          .draw_area = DrawArea {
            .viewport   = surface.viewport(),
            .draw_calls = draw_stream.get_stream(),
          },
        }
    );

    frame_ctx.command_list.end();
    rhi::gpu::submit_work(rhi::default_submit_info(frame_ctx, surface));
    surface.present();
  }
  rhi::gpu::wait_idle();
}
catch (std::exception const& e) {
  std::cerr << "Error: " << e.what() << '\n';
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unknown error occurred" << '\n';
  return EXIT_FAILURE;
}
