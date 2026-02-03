#pragma once

#include "reflect3d/graphics/vk/vk_barriers.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_pso.hpp"
#include "reflect3d/graphics/vk/vk_pso_builder.hpp"
#include "reflect3d/graphics/vk/vk_shader.hpp"
#include "reflect3d/graphics/vk/vk_surface.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"

namespace rf3d::gfx::vk {

struct FrameInfo { };

class Core {
public:
  Core() {
    // hardcoded TODO write pso cache
    auto const shader_path = std::filesystem::path {mono::assets_path} / "shaders" / "shader.slang";
    auto const shader      = Shader {load_shader_bytecode(shader_path)};

    auto pso = PipelineBuilder() //
                   .vertex_stage(shader) //
                   .fragment_stage(shader) //
                   .build(); //

    pso_cache.emplace_back(std::move(pso));
  }

  void render_surface(Surface& surface, [[maybe_unused]] FrameInfo const& frame_info) const {
    FrameIndex const frame_index = gpu::next_frame();
    auto const opt_image         = surface.next_image(frame_index);

    if (not opt_image) {
      return;
    }

    auto image = opt_image.value();

    auto const commands = gpu::record_commands({}, [this, &image, &surface](CommandBuffer const& cmd) {
      std::array const attachment_info = {defaults::attachament_info(image->view(), defaults::clear_color)};

      transition_image_layout(cmd, *image, rendering_layout);

      auto const render_area    = defaults::render_area(surface.resolution());
      auto const rendering_info = defaults::rendering_info(render_area, attachment_info);
      cmd.record_rendering(rendering_info, [this, &surface, &render_area](CommandBuffer const& cmd_buffer) {
        cmd_buffer.bind_pipeline(core::PipelineBindPoint::eGraphics, *this->pso_cache.at(0))
            .set_viewport(0, defaults::viewport(surface.resolution()))
            .set_scissor(0, render_area)
            .set_cull_mode(core::CullModeFlagBits::eBack)
            .set_front_face(core::FrontFace::eClockwise)
            .set_primitive_topology(core::PrimitiveTopology::eTriangleList)
            .set_line_width(1.0F)
            .draw(3, 1, 0, 0);
      });

      transition_image_layout(cmd, *image, present_layout);
    });

    gpu::submit_work(
        {std::addressof(*surface.present_semaphore(frame_index)), 1}, //
        {std::addressof(commands), 1}, //
        {std::addressof(*surface.render_semaphore()), 1}
    );

    surface.present();
  }

private:
  std::vector<Pipeline> pso_cache;
};


} // namespace rf3d::gfx::vk
