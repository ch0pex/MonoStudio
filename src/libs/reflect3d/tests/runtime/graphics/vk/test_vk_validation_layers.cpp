#include <reflect3d/graphics/vk/instance/vk_validation_layers.hpp>
#include <string_view>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

namespace {

TEST_SUITE_BEGIN("Vulkan");

TEST_CASE("Verify validation layer support") {
  using namespace std::string_literals;

  // Mock supported validation layers
  std::vector<rf3d::gfx::vk::core::LayerProperties> const supported_layers = {
    {
      .layerName             = "VK_LAYER_KHRONOS_validation"s,
      .specVersion           = rf3d::gfx::vk::core::ApiVersion10,
      .implementationVersion = 0,
      .description           = ""s,
    },
    {
      .layerName             = "VK_LAYER_LUNARG_standard_validation"s,
      .specVersion           = rf3d::gfx::vk::core::ApiVersion10,
      .implementationVersion = 0,
      .description           = ""s,
    },
  };

  // Mock required validation layers
  std::vector<std::string_view> const required_layers = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_LUNARG_standard_validation",
  };

  CHECK(rf3d::gfx::vk::check_validation_layer_support(supported_layers, required_layers));
}

TEST_CASE("Verify validation layer support - missing layer") {
  using namespace std::string_literals;
  std::vector<rf3d::gfx::vk::core::LayerProperties> const supported_layers = {
    {
      .layerName             = "VK_LAYER_KHRONOS_validation"s,
      .specVersion           = rf3d::gfx::vk::core::ApiVersion10,
      .implementationVersion = 0,
      .description           = ""s,
    },
  };

  std::vector<std::string_view> const required_layers = {
    "VK_LAYER_KHRONOS_validation",
    "VK_LAYER_MISSING",
  };

  CHECK_FALSE(rf3d::gfx::vk::check_validation_layer_support(supported_layers, required_layers));
}

TEST_SUITE_END();

} // namespace
