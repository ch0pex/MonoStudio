
#include <cstdint>
#include <reflect3d/graphics/core2/resource_state.hpp>
#include <reflect3d/graphics/vk2/gpu.hpp>
#include <reflect3d/graphics/vk2/surface.hpp>
#include <reflect3d/window/window.hpp>
#include <reflect3d/window/window_builder.hpp>

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>
#include "reflect3d/graphics/core2/renderpass_descriptor.hpp"
#include "reflect3d/graphics/vk2/buffer.hpp"
#include "reflect3d/graphics/vk2/draw_stream.hpp"
#include "reflect3d/input/input.hpp"

#include <print>

struct Vertex {
  rf3d::math::vec3 position {0.0F, 0.0F, 0.0F};
  rf3d::math::vec4 color {0.4F, 0.4F, 0.4F, 0.0F};
};
inline constexpr Vertex vertex1 {.position = {0.0F, -0.5F, 0.0F}, .color = {1.0F, 0.0F, 0.0F, 1.0F}};
inline constexpr Vertex vertex2 {.position = {0.5F, 0.5F, 0.0F}, .color = {0.0F, 1.0F, 0.0F, 1.0F}};
inline constexpr Vertex vertex3 {.position = {-0.5F, 0.5F, 0.0F}, .color = {0.0F, 0.0F, 1.0F, 1.0F}};
inline constexpr std::array vertices                  = {vertex1, vertex2, vertex3};
inline constexpr std::array<std::uint16_t, 6> indices = {0U, 1U, 2U, 2U, 3U, 0U};

static_assert(std::is_trivially_copyable_v<Vertex>, "Vertex must be trivially copyable");

int main() try {

  rf3d::vk::Surface surface {rf3d::WindowBuilder().default_callbacks().build()};

  rf3d::vk::VertexBuffer vertex_buffer {vertices};
  rf3d::vk::IndexBuffer index_buffer {indices};
  // rf3d::vk::PipelineState pso {};

  std::stop_token token = mono::ex::setup_signals();
  while (not token.stop_requested()) {
    rf3d::input::poll_events();

    auto& frame_ctx = rf3d::vk::Gpu::new_frame();
    auto* image     = surface.next_image(frame_ctx.index);

    if (image == nullptr) {
      continue;
    }

    // rf3d::vk::DrawStreamBuilder draw_stream {};
    // draw_stream.draw({
    //   .pso           = pso,
    //   .vertex_buffer = vertex_buffer,
    //   .index_buffer  = index_buffer,
    //   .draw_params   = {
    //       .index_count = static_cast<std::uint32_t>(index_buffer.size()),
    //   },
    // });
    //
    // frame_ctx.command_list.render_pass(
    //     rf3d::RenderPass {
    //       .debug_name = "Test Render Pass",
    //       .render_targets =
    //           rf3d::RenderTargets {
    //             rf3d::ColorTarget {.texture = *image, .final_state = rf3d::ResourceState::present},
    //           },
    //       .draw_area = rf3d::DrawArea {
    //         .render_area = rf3d::Rect2D {.offset = {0, 0}, .extent = {1920, 1080}},
    //         .draw_calls  = draw_stream.get_stream()
    //       },
    //     }
    // );

    rf3d::vk::Gpu::submit_frame(frame_ctx, {&surface});
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
