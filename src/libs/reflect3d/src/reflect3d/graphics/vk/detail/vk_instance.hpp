#pragma once

#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"
#include "reflect3d/window/window_types.hpp"

namespace rf3d::vk::detail {

/**
 * Creates a Vulkan surface for the given native window.
 *
 * @param window The native window handle.
 * @return A raii::SurfaceKHR representing the created Vulkan surface.
 */
[[nodiscard]] raii::SurfaceKHR create_surface(NativeWindow window);

/**
 * Retrieves the list of physical devices available on the system.
 *
 * @return A reference to a raii::PhysicalDevices object containing the physical devices.
 */
[[nodiscard]] raii::PhysicalDevices const& physical_devices();

/*
 * Creates a VMA allocator for the given physical device and logical device.
 *
 * @param physical_device The physical device for which to create the allocator.
 * @param device The logical device for which to create the allocator.
 * @return A VmaAllocator representing the created allocator.
 */
Allocator create_allocator(core::PhysicalDevice physical_device, core::Device device);

} // namespace rf3d::vk::detail
