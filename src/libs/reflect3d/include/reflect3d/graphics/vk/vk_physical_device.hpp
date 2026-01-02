#pragma once

#include "reflect3d/graphics/vk/vk_gpu_families.hpp"


namespace rf3d::gfx::vk {

inline std::vector<char const*> physical_device_extensions() {
  return {
    core::KHRSwapchainExtensionName,
    core::KHRSpirv14ExtensionName,
    core::KHRSynchronization2ExtensionName,
    core::KHRCreateRenderpass2ExtensionName,
  };
}

class PhysicalDevice {
public:
  using native_type      = raii::PhysicalDevice;
  using logical_type     = raii::Device;
  using queues_info_type = GpuQueueFamilies;

  explicit PhysicalDevice(native_type const& handle) : native(handle), queues_info(handle) { }

  [[nodiscard]] logical_type create_logical_device() const { return create_logical_device(queues_info); }

  [[nodiscard]] logical_type create_logical_device(queues_info_type const& info) const {
    core::PhysicalDeviceExtendedDynamicStateFeaturesEXT extended_dynamic_state_features {
      .extendedDynamicState = core::True,
    };

    core::PhysicalDeviceVulkan13Features vulkan13_features {
      .pNext            = &extended_dynamic_state_features,
      .dynamicRendering = core::True,
    };

    auto features  = native.getFeatures2();
    features.pNext = &vulkan13_features;

    auto const queue_create_infos = info.device_creation_infos();

    // Needs static live time
    static auto const extensions = physical_device_extensions();
    core::DeviceCreateInfo create_info {
      .pNext                   = &features,
      .queueCreateInfoCount    = static_cast<std::uint32_t>(queue_create_infos.size()),
      .pQueueCreateInfos       = queue_create_infos.data(),
      .enabledExtensionCount   = static_cast<std::uint32_t>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data(),
    };


    return native.createDevice(create_info);
  }

  [[nodiscard]] queues_info_type const& queue_creation_info() const noexcept { return queues_info; }


private:
  native_type native;
  queues_info_type queues_info;
};

} // namespace rf3d::gfx::vk
