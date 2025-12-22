#pragma once

#include "reflect3d/graphics/instance_config.hpp"
#include "reflect3d/graphics/vk/vk_extensions.hpp"
#include "reflect3d/graphics/vk/vk_instance_detail.hpp"
#include "reflect3d/graphics/vk/vk_validation_layers.hpp"

#include <vulkan/vulkan_core.h>

namespace rf3d::hri::vk {

class Instance {
public:
  explicit Instance(InstanceConfig const& config = {}) : instance(detail::create_instance(config)) { }

  ~Instance() { vkDestroyInstance(instance, nullptr); }

  Instance(Instance const&) = default;

  Instance(Instance&&) = delete;

  Instance& operator=(Instance const&) = default;

  Instance& operator=(Instance&&) = delete;

private:
  VkInstance instance {};
};

} // namespace rf3d::hri::vk
