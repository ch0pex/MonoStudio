#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/window/window_types.hpp"

namespace rf3d::gfx::vk::instance {

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


} // namespace rf3d::gfx::vk::instance
