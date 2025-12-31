#pragma once

#include "reflect3d/graphics/vk/vk_graphics_families.hpp"


namespace rf3d::gfx::vk {

class PhysicalDevice : public raii::PhysicalDevice {
public:
  using handle_type  = raii::PhysicalDevice;
  using indices_type = QueueFamilyIndices;

  explicit PhysicalDevice(handle_type const& handle) :
    raii::PhysicalDevice(handle), indices(detail::find_queue_families(*this)) { }

  [[nodiscard]] indices_type const& queue_indices() const noexcept { return indices; }

private:
  indices_type indices;
};

} // namespace rf3d::gfx::vk
