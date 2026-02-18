#pragma once

#include "reflect3d/graphics/core/mesh.hpp"
#include "reflect3d/graphics/vk/memory/vk_index_buffer.hpp"
#include "reflect3d/graphics/vk/memory/vk_vertex_buffer.hpp"
#include "reflect3d/graphics/vk/vk_barriers.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_pso.hpp"
#include "reflect3d/graphics/vk/vk_pso_builder.hpp"
#include "reflect3d/graphics/vk/vk_shader.hpp"
#include "reflect3d/graphics/vk/vk_surface.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"

#include <mono/misc/as_span.hpp>

namespace rf3d::gfx::vk {


struct FrameInfo { };

struct StaticMesh {
  VertexBuffer vertex_buffer;
  IndexBuffer index_buffer;
};

class Core {
public:
  Core() {
    // TODO: hardcoded, write pso cache
    auto const shader_path = std::filesystem::path {mono::assets_path} / "shaders" / "basic_shader.slang";
    auto const shader      = Shader {load_shader_bytecode(shader_path)};

    auto pso = PipelineBuilder() //
                   .vertex_stage(shader) //
                   .fragment_stage(shader) //
                   .build(); //

    pso_cache.emplace_back(std::move(pso));
    meshes.reserve(10);
  }

  ~Core() {
    LOG_INFO("Waiting for gpu to be idle before destroying VkGraphics backend");
    gpu::wait_idle();
    LOG_INFO("VkGraphics backend destroyed");
  }

  void render_surface(Surface& surface, [[maybe_unused]] FrameInfo const& frame_info) const {
    FrameIndex const frame_index = gpu::next_frame();
    auto const opt_image         = surface.next_image(frame_index);

    if (not opt_image) {
      return;
    }

    // NOTE: using proxy until std::optional<T&>
    auto image          = opt_image.value();
    auto const commands = gpu::record_commands({}, [this, &image, &surface](CommandBuffer const& cmd) {
      std::array const attachment_info = {defaults::attachament_info(image->view(), defaults::clear_color)};
      auto const render_area           = defaults::render_area(surface.resolution());
      auto const rendering_info        = defaults::rendering_info(render_area, attachment_info);

      transition_image_layout(cmd, *image, rendering_layout);

      cmd.record_rendering(rendering_info, [this, &surface, &render_area](CommandBuffer const& cmd_buffer) {
        for (auto const& mesh: meshes) {
          cmd_buffer.bind_pipeline(core::PipelineBindPoint::eGraphics, *this->pso_cache.at(0))
              .bind_vertex_buffer(0, mesh.vertex_buffer.handle(), 0)
              .bind_index_buffer(mesh.index_buffer.handle(), 0, core::IndexType::eUint16)
              .set_viewport(0, defaults::viewport(surface.resolution()))
              .set_scissor(0, render_area)
              .set_cull_mode(core::CullModeFlagBits::eBack)
              .set_front_face(core::FrontFace::eClockwise)
              .set_primitive_topology(core::PrimitiveTopology::eTriangleList)
              .set_line_width(1.0F)
              .draw_indexed(mesh.index_buffer.size(), 1, 0, 0, 0);
        }
      });

      transition_image_layout(cmd, *image, present_layout);
    });

    core::PipelineStageFlags mask {core::PipelineStageFlagBits::eColorAttachmentOutput};
    auto submit_info = gpu::SubmitInfo {
      .wait_semaphores     = mono::as_span(*surface.present_semaphore(frame_index)),
      .wait_dst_stage_mask = mono::as_span(mask),
      .command_buffers     = mono::as_span(commands),
      .signal_semaphores   = mono::as_span(*surface.render_semaphore()),
    };
    gpu::submit_work(submit_info, gpu::wait::fence);
    surface.present();
  }

  void add_mesh(Mesh const& mesh) { //
    meshes.emplace_back(VertexBuffer {mesh.vertices}, IndexBuffer {mesh.indices});
  }

private:
  std::vector<Pipeline> pso_cache;
  std::vector<StaticMesh> meshes;
};


} // namespace rf3d::gfx::vk
