#pragma once

#include "reflect3d/graphics/core/primitive_types.hpp"
#include "reflect3d/window/utils/resolution.hpp"
#include "reflect3d/window/window.hpp"

namespace rf3d::archetypes {

/**
 * Surface archetype that models a GPU presentation surface.
 *
 * Since C++ does not support universal quantification, this mock type
 * can be used by concepts to trick this language limitation.
 */
struct Surface {
  struct Image { };

  using resolution_type = rf3d::Resolution;
  using image_type      = Image;
  using window_type     = Window;

  Surface() = default;

  Surface(window_type&& window);

  Surface(Surface const&) = delete;

  Surface(Surface&&) = default;

  Surface& operator=(Surface const&) = delete;

  Surface& operator=(Surface&&) = default;

  ~Surface() = default;

  [[nodiscard]] image_type next_image(FrameIndex frame_index);

  void present();

  void resize(resolution_type const& resolution);
};

} // namespace rf3d::archetypes
