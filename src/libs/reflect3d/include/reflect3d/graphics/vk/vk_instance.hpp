#pragma once

#include "reflect3d/graphics/vk/vk_instance_types.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"

#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk {

class Instance {
public:
  /*********************
   *    Type Traits    *
   ********************/

  using underlying_type = std::conditional_t<enable_validation_layers, detail::DebugInstance, detail::ReleaseInstance>;

  /**************************
   *    Member functions    *
   **************************/


private:
  underlying_type handle;
};

} // namespace rf3d::hri::vk
