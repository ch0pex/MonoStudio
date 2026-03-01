#pragma once

#include "reflect3d/graphics/concepts.hpp"
#include "reflect3d/graphics/core/frame_info.hpp"
#include "reflect3d/window/window.hpp"

#include <mono/containers/hive.hpp>

namespace rf3d::gfx {

template<RenderBackend Backend>
class Surface {
public:
  using backend_type = typename Backend::surface_handle;
  using window_type  = Window;

  Surface(Window&& window) : surface_handle(Backend::create_surface(std::move(window))) { }

  Surface(Surface const&) = delete;

  Surface(Surface&&) = default;

  Surface& operator=(Surface const&) = delete;

  Surface& operator=(Surface&&) = default;

  ~Surface() { Backend::destroy_surface(surface_handle); }

  void render(FrameInfo const& frame_info) { Backend::render_surface(surface_handle, frame_info); }

  [[nodiscard]] bool should_close() const { return surface_handle->should_close(); }

  // void resize(Resolution const& new_resolution) { surface_handle->resize(new_resolution); }
  //
  // void resize_window(Resolution const& new_resolution) { surface_handle->resize_window(new_resolution); }
  //
  // void resize_framebuffer(Resolution const& new_resolution) { surface_handle->resize_framebuffer(new_resolution); }
  //
  // Resolution window_size() { return surface_handle->window_size(); }
  //
  // void framebuffer_resolution() { return surface_handle->framebuffer_resolution(); }

private:
  backend_type surface_handle;
};

} // namespace rf3d::gfx
