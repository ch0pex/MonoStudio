#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace rf3d::gfx::vk {

namespace core = ::vk;
namespace raii = ::vk::raii;

using FrameIndex = std::uint32_t;
using ImageIndex = std::uint32_t;

} // namespace rf3d::gfx::vk
