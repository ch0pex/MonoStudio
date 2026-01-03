#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"

#include <ranges>

namespace rf3d::gfx::vk {

class Image {
public:
  using config_type = core::ImageViewCreateInfo;
  using native_type = core::Image;
  using view_type   = raii::ImageView;
  using device_type = raii::Device;

  explicit Image(native_type const native_handle, view_type&& view) : handle(native_handle), view(std::move(view)) { }


private:
  native_type handle;
  view_type view;
};

} // namespace rf3d::gfx::vk
