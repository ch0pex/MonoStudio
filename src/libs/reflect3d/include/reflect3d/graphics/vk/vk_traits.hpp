#pragma once


#include "reflect3d/graphics/vk/vk_instance.hpp"

namespace rf3d::hri::vk {

// class Vulkan {
// public:
//   explicit Vulkan(vk::Instance const& instance) : instance(instance) { }
//
// private:
//   vk::Instance instance;
// };

struct Traits {
  using instance_type = Instance;
};

} // namespace rf3d::hri::vk
