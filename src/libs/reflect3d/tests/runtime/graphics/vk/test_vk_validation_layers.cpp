#include <reflect3d/graphics/vk/vk_validation_layers.hpp>
#include <string_view>
#include <vector>
#include <vulkan/vulkan_core.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

namespace {

TEST_SUITE_BEGIN("Vulkan");

TEST_CASE("Verify validation layer support") {
  // Mock supported validation layers
  std::vector<VkLayerProperties> const supported_layers = {
    {
      .layerName             = "VK_LAYER_KHRONOS_validation",
      .specVersion           = VK_API_VERSION_1_0,
      .implementationVersion = 0,
      .description           = "",
    },
    {
      .layerName             = "VK_LAYER_LUNARG_standard_validation",
      .specVersion           = VK_API_VERSION_1_0,
      .implementationVersion = 0,
      .description           = "",
    },
  };

  // Mock required validation layers
  std::vector<std::string_view> const required_layers = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_standard_validation",
  };

  CHECK(rf3d::hri::vk::check_validation_layer_support(supported_layers, required_layers));
}

TEST_CASE("Verify validation layer support - missing layer") {
  std::vector<VkLayerProperties> const supported_layers = {
    {
      .layerName             = "VK_LAYER_KHRONOS_validation",
      .specVersion           = VK_API_VERSION_1_0,
      .implementationVersion = 0,
      .description           = "",
    },
  };

  std::vector<std::string_view> const required_layers = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_MISSING",
  };

  CHECK_FALSE(rf3d::hri::vk::check_validation_layer_support(supported_layers, required_layers));
}

TEST_SUITE_END();

} // namespace
