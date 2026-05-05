#pragma once

#include "reflect3d/graphics/api/archetypes/surface.hpp"

namespace rf3d::archetypes {

/**
 * Gpu archetype that models a GPU device.
 *
 * Since C++ does not support universal quantification, this mock type
 * can be used by concepts to trick this language limitation.
 */
struct Gpu {
  struct FrameContext {
    using command_list_type = CommandList;
    using fence_type        = Fence;
    command_list_type command_list;
    fence_type fence;
  };

  using frame_context_type = FrameContext;
  using surface_type       = Surface;

  Gpu()                      = delete;
  Gpu(Gpu const&)            = delete;
  Gpu(Gpu&&)                 = delete;
  Gpu& operator=(Gpu const&) = delete;
  Gpu& operator=(Gpu&&)      = delete;

  ~Gpu() = default;

  static void wait_idle();

  static frame_context_type& new_frame();

  static void submit_frame(frame_context_type& frame_ctx, mono::span<surface_type* const> surfaces);
};

} // namespace rf3d::archetypes
