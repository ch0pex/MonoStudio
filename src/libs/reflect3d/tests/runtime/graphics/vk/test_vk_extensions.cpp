#include <reflect3d/graphics/vk/vk_extensions.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Vulkan");

TEST_CASE("Verify extensions support") {
  // Mock supported extensions
  std::vector<rf3d::gfx::vk::core::ExtensionProperties> supported_extensions = {
    {.extensionName = std::string {"VK_KHR_surface"}, .specVersion = rf3d::gfx::vk::core::ApiVersion10},
    {.extensionName = std::string {"VK_KHR_win32_surface"}, .specVersion = rf3d::gfx::vk::core::ApiVersion10},
  };

  // Mock required extensions
  std::vector<std::string_view> required_extensions = {
    "VK_KHR_surface",
    "VK_KHR_win32_surface",
  };

  CHECK(rf3d::gfx::vk::check_extensions_support(supported_extensions, required_extensions));
}

TEST_CASE("Verify extensions support - missing extension") {
  std::vector<rf3d::gfx::vk::core::ExtensionProperties> supported_extensions = {
    {.extensionName = std::string {"VK_KHR_surface"}, .specVersion = rf3d::gfx::vk::core::ApiVersion10},
  };

  std::vector<std::string_view> required_extensions = {
    "VK_KHR_surface",
    "VK_KHR_missing_extension",
  };

  CHECK_FALSE(rf3d::gfx::vk::check_extensions_support(supported_extensions, required_extensions));
}

TEST_SUITE_END();
