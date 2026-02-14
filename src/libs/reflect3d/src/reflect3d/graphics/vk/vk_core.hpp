#pragma once

#include "reflect3d/graphics/vk/memory/vk_index_buffer.hpp"
#include "reflect3d/graphics/vk/memory/vk_vertex_buffer.hpp"
#include "reflect3d/graphics/vk/vk_barriers.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_pso.hpp"
#include "reflect3d/graphics/vk/vk_pso_builder.hpp"
#include "reflect3d/graphics/vk/vk_shader.hpp"
#include "reflect3d/graphics/vk/vk_surface.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"
#include "reflect3d/render/mesh.hpp"

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

    // TODO: improve API by creating my onw submit info structure
    gpu::submit_work(
        mono::as_span(*surface.present_semaphore(frame_index)), //
        mono::as_span(commands), //
        mono::as_span(*surface.render_semaphore())
    );
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
