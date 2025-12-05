#pragma once

#include "rflect3d/graphics/vk/vk_instance.hpp"

namespace rflect::gfx {


/**
 * Main vulkan context class this class
 * must follow HRI concept
 *
 */
class Vulkan {
public:
  Vulkan() = default;

private:
  vk::Instance instance;
};


} // namespace rflect::gfx
