#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"


namespace rf3d::gfx::vk {

class Image {
public:
  using config_type  = core::ImageViewCreateInfo;
  using native_type  = core::Image;
  using view_type    = raii::ImageView;
  using device_type  = raii::Device;
  using barrier_type = core::ImageMemoryBarrier2;

  explicit Image(native_type const native_handle, view_type&& view) :
    handle(native_handle), image_view(std::move(view)) { }

  native_type const& operator*() const noexcept { return handle; }

  [[nodiscard]] view_type const& view() const noexcept { return image_view; }

private:
  native_type handle;
  view_type image_view;
};

} // namespace rf3d::gfx::vk
