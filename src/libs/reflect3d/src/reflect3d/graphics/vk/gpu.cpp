
#include "reflect3d/graphics/vk/gpu.hpp"
#include <vulkan/vulkan_enums.hpp>
#include "reflect3d/graphics/core/defaults.hpp"
#include "reflect3d/graphics/core/primitive_types.hpp"
#include "reflect3d/graphics/vk/detail/vk_gpu_detail.hpp"
#include "reflect3d/graphics/vk/surface.hpp"

namespace rf3d::vk {

namespace {

class FrameContextManager {
public:
  using context_type = FrameContext;
  using index_type   = FrameIndex;

  FrameContextManager() {
    for (auto [index, ctx]: std::views::enumerate(frames_context)) {
      ctx.index = static_cast<std::uint32_t>(index);
    }
  }

  FrameContext& next_frame() { //
    return frames_context.at(++frame_index % defaults::max_frames_in_flight);
  }

  [[nodiscard]] FrameContext& current_frame() { //
    return frames_context.at(frame_index);
  }

  [[nodiscard]] index_type current_frame_index() const noexcept { //
    return frame_index;
  }

private:
  index_type frame_index {};
  std::array<context_type, defaults::max_frames_in_flight> frames_context {};
};

// class DeletionQueue {
// public:
//   using value_type = std::variant<detail::>;
//
//   void push(value_type resource) { resources[current_frame()].emplace_back(std::move(resource)); }
//
//   void collect_garbage() { resources[current_frame()].clear(); }
//
//   std::array<std::vector<value_type>, frames_in_flight> resources;
// private:
// };


FrameContextManager& frame_manager() {
  static FrameContextManager instance;
  return instance;
}

} // namespace

void Gpu::wait_idle() { //
  detail::wait_idle();
}

FrameIndex Gpu::current_frame_index() { //
  return frame_manager().current_frame_index();
}

Gpu::frame_context_type& Gpu::new_frame() {
  // assert(); TODO: check if frame was submitted before starting a new one
  auto& ctx = frame_manager().next_frame();
  ctx.fence.wait();

  // TODO: collect garbage resources from previous frame and free them here

  ctx.fence.reset();
  ctx.command_list.reset();
  ctx.command_list.begin();
  return ctx;
}

// TODO: optimize this function
void Gpu::submit_frame(Gpu::frame_context_type& frame_ctx, mono::span<surface_type* const> surfaces [[maybe_unused]]) {

  auto to_stage_flag       = [&](std::uint32_t) { return detail::core::PipelineStageFlagBits::eColorAttachmentOutput; };
  auto to_render_semaphore = [&](surface_type const* surface) { return *surface->render_semaphore().handle(); };
  auto to_present_semaphore = [&](surface_type const* surface) {
    return *surface->present_semaphore(frame_ctx.index).handle();
  };

  auto wait_semaphores = surfaces //
                         | std::views::transform(to_present_semaphore) //
                         | std::ranges::to<std::vector>();

  auto signal_semaphores = surfaces //
                           | std::views::transform(to_render_semaphore) //
                           | std::ranges::to<std::vector>();

  auto masks = std::views::iota(0U, static_cast<std::uint32_t>(surfaces.size())) //
               | std::views::transform(to_stage_flag) //
               | std::ranges::to<std::vector<detail::core::PipelineStageFlags>>();

  frame_ctx.command_list.end();
  detail::submit_work(
      {
        .wait_semaphores     = wait_semaphores,
        .wait_dst_stage_mask = masks,
        .command_buffers     = {frame_ctx.command_list.handle()},
        .signal_semaphores   = signal_semaphores,
      },
      *frame_ctx.fence.handle()
  );
}

void Gpu::submit_frame(frame_context_type& frame_ctx, surface_type const& surfaces) {
  detail::core::PipelineStageFlags mask = detail::core::PipelineStageFlagBits::eColorAttachmentOutput;
  frame_ctx.command_list.end();
  detail::submit_work(
      {
        .wait_semaphores     = {*surfaces.present_semaphore(frame_ctx.index).handle()},
        .wait_dst_stage_mask = {mask},
        .command_buffers     = {frame_ctx.command_list.handle()},
        .signal_semaphores   = {*surfaces.render_semaphore().handle()},
      },
      *frame_ctx.fence.handle()
  );
}

} // namespace rf3d::vk
