#pragma once

#ifdef MONO_USE_CPP_MODULES
import vulkan_hpp;
import std;
#else
#include <cstdint>
#include <vulkan/vulkan_raii.hpp>
#endif


namespace rf3d::gfx::vk {

namespace core = ::vk;
namespace raii = ::vk::raii;

using FrameIndex = std::uint32_t;
using ImageIndex = std::uint32_t;

} // namespace rf3d::gfx::vk

// clang-format on
