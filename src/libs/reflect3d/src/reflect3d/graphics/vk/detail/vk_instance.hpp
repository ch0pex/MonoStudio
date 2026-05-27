/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vk_instance.hpp
 * @date 26/05/2026
 * @brief Vulkan instance utility functions for surface, physical device and allocator creation
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"
#include "reflect3d/window/window_types.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


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
