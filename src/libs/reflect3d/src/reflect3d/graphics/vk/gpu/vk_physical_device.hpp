#pragma once

#include "reflect3d/graphics/vk/gpu/vk_gpu_families.hpp"
#include "reflect3d/graphics/vk/vk_surface_info.hpp"


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
  using feature_chain_type = core::StructureChain< //
      core::PhysicalDeviceFeatures2,  //
      core::PhysicalDeviceVulkan11Features,  //
      core::PhysicalDeviceVulkan13Features, //
      core::PhysicalDeviceExtendedDynamicStateFeaturesEXT //
  >;

  explicit PhysicalDevice(native_type const& handle) : native(handle), queues_info(handle) { }

  [[nodiscard]] logical_type create_logical_device() const { return create_logical_device(queues_info); }

  [[nodiscard]] logical_type create_logical_device(queues_info_type const& info) const {

    auto const queue_create_infos = info.device_creation_infos();

    // Needs static live time
    static auto const extensions = physical_device_extensions();

    feature_chain_type featureChain = {
      {}, // core::PhysicalDeviceFeatures2
      {.shaderDrawParameters = core::True}, // core::PhysicalDeviceVulkan11Features
      {.synchronization2 = core::True, .dynamicRendering = core::True}, // core::PhysicalDeviceVulkan13Features
      {.extendedDynamicState = core::True} // core::PhysicalDeviceExtendedDynamicStateFeaturesEXT
    };

    // create a Device
    core::DeviceCreateInfo create_info {
      .pNext                   = &featureChain.get<core::PhysicalDeviceFeatures2>(),
      .queueCreateInfoCount    = static_cast<std::uint32_t>(queue_create_infos.size()),
      .pQueueCreateInfos       = queue_create_infos.data(),
      .enabledExtensionCount   = static_cast<std::uint32_t>(extensions.size()),
      .ppEnabledExtensionNames = extensions.data(),
    };

    return native.createDevice(create_info);
  }

  [[nodiscard]] queues_info_type const& queue_creation_info() const noexcept { return queues_info; }

  [[nodiscard]] SurfaceInfo get_surface_info(core::SurfaceKHR const& surface) const {
    return {
      .capabilities  = native.getSurfaceCapabilitiesKHR(surface),
      .formats       = native.getSurfaceFormatsKHR(surface),
      .present_modes = native.getSurfacePresentModesKHR(surface),
    };
  }

private:
  native_type native;
  queues_info_type queues_info;
};

} // namespace rf3d::gfx::vk
