#pragma once

namespace rf3d::gfx::archetypes {

/**
 * Gpu archetype that models a GPU device.
 *
 * Since C++ does not support universal quantification, this mock type
 * can be used by concepts to trick this language limitation.
 */
struct Gpu {
  struct FrameContext { };

  using frame_context_type = FrameContext;

  Gpu()                          = delete;
  Gpu(Gpu const&)                = delete;
  Gpu(Gpu&&)                     = delete;
  Gpu& operator=(Gpu const&)     = delete;
  Gpu& operator=(Gpu&&)          = delete;

  ~Gpu() = default;

  static void wait_idle();

  static frame_context_type& new_frame();

  static void submit_frame();
};

} // namespace rf3d::gfx::archetypes
