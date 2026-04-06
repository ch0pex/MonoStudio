
#include <reflect3d/graphics/core2/renderpass_descriptor.hpp>
#include <reflect3d/graphics/core2/resource_state.hpp>
#include <reflect3d/graphics/core2/shader/compiler.hpp>
#include <reflect3d/graphics/core2/shader/targets.hpp>
#include <reflect3d/graphics/vk2/buffer.hpp>
#include <reflect3d/graphics/vk2/draw_stream.hpp>
#include <reflect3d/graphics/vk2/gpu.hpp>
#include <reflect3d/graphics/vk2/pso.hpp>
#include <reflect3d/graphics/vk2/surface.hpp>
#include <reflect3d/input/input.hpp>
#include <reflect3d/window/window.hpp>
#include <reflect3d/window/window_builder.hpp>

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>
#include "assets_path.hpp"

struct Vertex {
  rf3d::math::vec3 position {0.0F, 0.0F, 0.0F};
  rf3d::math::vec4 color {0.4F, 0.4F, 0.4F, 0.0F};
};

std::array constexpr vertices = {
  Vertex {.position = {-0.5F, -0.5F, 0.F}, .color {1.0F, 0.0F, 0.0F, 0.0F}},
  Vertex {.position = {0.5F, -0.5F, 0.F}, .color = {0.0F, 1.0F, 0.0F, 0.F}},
  Vertex {.position = {0.5F, 0.5F, 0.F}, .color = {0.0F, 0.0F, 1.0F, 0.F}},
  Vertex {.position = {-0.5F, 0.5F, 0.F}, .color = {1.0F, 1.0F, 1.0F, 0.F}}
};
inline constexpr std::array<std::uint16_t, 6> indices = {0, 1, 2, 2, 3, 0};

int main() try {
  auto const shader_path     = std::filesystem::path {mono::assets_path} / "shaders" / "basic_shader.slang";
  auto const shader_bytecode = rf3d::shader::compile_module<rf3d::shader::SpirV>(shader_path);

  rf3d::vk::VertexBuffer vertex_buffer {vertices};
  rf3d::vk::IndexBuffer index_buffer {indices};

  rf3d::vk::PipelineState pso {{
    .debug_name      = "Triangle PSO",
    .vertex_shader   = {.bytecode = shader_bytecode},
    .fragment_shader = {.bytecode = shader_bytecode},
    .rasterizer_state =
        {
          .cull_mode = rf3d::CullMode::none,
        },
    .vertex_buffer_bindings = {
      {
        .byte_stride = sizeof(Vertex),
        .attributes  = {
          {.offset = offsetof(Vertex, position), .format = rf3d::Format::rgb32_sfloat},
          {.offset = offsetof(Vertex, color), .format = rf3d::Format::rgba32_sfloat},
        },
      },
    },
  }};


  rf3d::vk::Surface surface {rf3d::WindowBuilder().default_callbacks().build()};
  std::stop_token token = mono::ex::setup_signals();
  while (not token.stop_requested()) {
    rf3d::input::poll_events();

    auto& frame_ctx = rf3d::vk::Gpu::new_frame();
    auto* image     = surface.next_image(frame_ctx.index);

    if (image == nullptr) {
      continue;
    }

    rf3d::vk::DrawStreamBuilder draw_stream {};
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
            .draw_calls = draw_stream.get_stream(),
          },
        }
    );

    rf3d::vk::Gpu::submit_frame(frame_ctx, surface);
    surface.present();
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

// 1. Render semaphore double-signal race (likely the freeze cause)
//
//  With separate graphics and present queues, the timeline looks like this:
//
//  Graphics Q: [Frame 0 submit → signals render_sems A,B,C + fence1]
//  Present  Q: [present s0 waiting on A] [present s1 waiting on B] [present s2 waiting on C]
//
//  fence1 signals when the submit completes, NOT when the presents consume the render semaphores. With an empty command
//  buffer, the submit completes almost immediately. So:
//
//  GPU: fence1 signals (submit done, render_sems signaled but NOT YET consumed by presents)
//  CPU: fence1 wait returns → Frame 2 starts → submit signals render_sems AGAIN
//
//  If Frame 2 acquires the same swapchain images as Frame 0 (possible when cycling with mailbox mode), it tries to
//  re-signal render_sem_A/B/C while Frame 0's presents are still pending in the present queue, waiting to consume them.
//  Signaling an already-signaled binary semaphore with a pending wait is a Vulkan spec violation → GPU hang → fence
//  never signals → frozen.
//
//  With one submit per surface (working case), the present for each surface immediately follows its own submit on the
//  same command stream, dramatically reducing this window.
//
//  2. Semaphore index mismatch (fragile design)
//
//  // present_semaphore: indexed by frame_index (0 or 1)
//  images.at(frame_index).present_semaphore   // in acquireNextImage
//
//  // render_semaphore: indexed by current_image_index (0, 1, or 2)
//  images.at(current_image_index).render_semaphore  // in submit + present
//
//  The images array has 3 entries (minImageCount ≥ 3). The present_sem only ever uses slots 0 and 1, but render_sem
//  uses all 3. This means images[2].render_semaphore can be signaled
//   and waited on, but images[2].present_semaphore is never used for acquisition — they live in the same struct but are
//   accessed via inconsistent indices. This creates more surface
//  area for the race above.
//
//  3. poll_events() inside the surface loop
//
//  for (auto& surface: surfaces) {
//      rf3d::input::poll_events();         // polls all windows' events
//      auto* image = surface.next_image(frame_ctx.index);  // can block on acquireNextImage
//
//  Events are polled BEFORE each acquireNextImage, but if one blocks for a vsync period (~16ms), all 3 windows are
//  unresponsive during that time. Move poll_events() outside the surface loop.
//
//  ---
//  The fix
//
//  The combined-submit pattern requires a mechanism to guarantee the present queue has consumed render semaphores
//  before they're re-signaled. One approach: use a timeline semaphore per surface to track present completion, and wait
//  on it before re-using render semaphores. The simpler fix matching your working case is keeping per-surface submits —
//  one command
//   buffer per surface, which naturally serializes acquire → render → signal → present.
//
//  If you want the combined approach to work correctly, you should also batch the presents into a single
//  vkQueuePresentKHR with all 3 swapchains in one call (instead of 3 separate calls), which reduces present queue
//  pressure:
//
//  // Instead of 3 separate vkQueuePresentKHR calls, batch them:
//  // core::PresentInfoKHR with pSwapchains[3] and pWaitSemaphores[3]
//
//  This still doesn't fix the race, but combining it with tracking whether images cycled through the full FIFO/mailbox
//  round-trip (e.g., via a binary semaphore that the present signals, waited on before re-using render_sems) would make
//  it robust.
//
//
