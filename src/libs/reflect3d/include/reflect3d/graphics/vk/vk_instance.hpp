#pragma once

#include "reflect3d/graphics/vk/vk_debug_instance.hpp"

namespace rf3d::hri::vk {

/**
 * Vulkan instance
 *
 * @note if validation layers are enabled DebugInstance is used instead
 */
using Instance = std::conditional_t<enable_validation_layers, DebugInstance, BaseInstance>;

} // namespace rf3d::hri::vk
