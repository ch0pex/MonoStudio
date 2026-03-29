#include "reflect3d/graphics/vk/vk_renderer.hpp"

#include "mono/containers/hive.hpp"
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

namespace {

struct StaticMesh {
  VertexBuffer vertex_buffer;
  IndexBuffer index_buffer;
};

struct Context {
  Context() {
    // TODO: hardcoded, write pso cache logic
    auto const shader_path = std::filesystem::path {mono::assets_path} / "shaders" / "basic_shader.slang";
    auto const shader      = Shader {load_shader_bytecode(shader_path)};

    auto pso = PipelineBuilder().vertex_stage(shader).fragment_stage(shader).build();

    pso_cache.emplace_back(std::move(pso));
    meshes.reserve(10);
  }

  Context(Context const&) = default;

  Context(Context&&) = delete;

  Context& operator=(Context const&) = default;

  Context& operator=(Context&&) = delete;

  ~Context() { gpu::wait_idle(); }

  std::vector<Pipeline> pso_cache;
  std::vector<StaticMesh> meshes;
  SurfaceContainer surfaces;
};

Context& renderer_context() {
  static Context impl;
  return impl;
}

} // namespace

Renderer::surface_handle Renderer::create_surface(Window&& window) { //
  auto& context = renderer_context();
  return context.surfaces.emplace(std::move(window));
}

void Renderer::destroy_surface(Renderer::surface_handle const& surface) {
  auto& context = renderer_context();
  context.surfaces.erase(surface);
}

void Renderer::render_surface(Renderer::surface_handle const& surface, [[maybe_unused]] FrameInfo const& frame_info) {
  FrameIndex const frame_index = gpu::next_frame();
  auto const opt_image         = surface->next_image(frame_index);

  auto image [[maybe_unused]] = opt_image.value();
  auto const commands         = gpu::record_commands({}, [](CommandBuffer const& cmd [[maybe_unused]]) { });

  core::PipelineStageFlags mask {core::PipelineStageFlagBits::eColorAttachmentOutput};
  auto submit_info = gpu::SubmitInfo {
    .wait_semaphores     = {*surface->present_semaphore(frame_index)},
    .wait_dst_stage_mask = {mask},
    .command_buffers     = {commands},
    .signal_semaphores   = {surface->render_semaphore()},
  };

  gpu::submit_work(submit_info, gpu::wait::fence);
  surface->present();
}

void Renderer::add_mesh(Mesh const& mesh) {
  renderer_context().meshes.emplace_back(VertexBuffer {mesh.vertices}, IndexBuffer {mesh.indices});
}

} // namespace rf3d::gfx::vk
