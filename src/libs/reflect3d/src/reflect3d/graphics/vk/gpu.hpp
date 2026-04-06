#pragma once

#include "reflect3d/graphics/vk/command_list.hpp"
#include "reflect3d/graphics/vk/fence.hpp"

#include "reflect3d/graphics/core/primitive_types.hpp"
#include "reflect3d/graphics/vk/fwd.hpp"

namespace rf3d::vk {

struct FrameContext {
  FrameIndex index {};
  GraphicsCommandList command_list {};
  Fence fence {};
};

struct Gpu {
  // --- Type Traits ---

  using frame_context_type = FrameContext;
  using surface_type       = Surface;

  // ---  Constructors ---

  Gpu() = delete;

  Gpu(Gpu const&) = delete;

  Gpu(Gpu&&) = delete;

  Gpu& operator=(Gpu const&) = delete;

  Gpu& operator=(Gpu&&) = delete;

  ~Gpu() = default;

  // --- Member Functions ---

  static void wait_idle();

  static frame_context_type& new_frame();

  static FrameIndex current_frame_index();

  static void submit_frame(frame_context_type& frame_ctx, mono::span<surface_type* const> surfaces);

  static void submit_frame(frame_context_type& frame_ctx, surface_type const& surfaces);
};

} // namespace rf3d::vk
