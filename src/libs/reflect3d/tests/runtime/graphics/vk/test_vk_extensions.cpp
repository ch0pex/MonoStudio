#include <reflect3d/graphics/vk/vk_instance.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Vulkan");

TEST_CASE("Verify extensions support") {
  // Mock supported extensions
  std::vector<VkExtensionProperties> supported_extensions = {
    {.extensionName = "VK_KHR_surface", .specVersion = VK_API_VERSION_1_0},
    {.extensionName = "VK_KHR_win32_surface", .specVersion = VK_API_VERSION_1_0},
  };

  // Mock required extensions
  std::vector<char const*> required_extensions = {
    "VK_KHR_surface",
    "VK_KHR_win32_surface",
  };

  CHECK(rf3d::hri::vk::verify_extensions_support(supported_extensions, required_extensions));
}

TEST_CASE("Verify extensions support - missing extension") {
  std::vector<VkExtensionProperties> supported_extensions = {
    {.extensionName = "VK_KHR_surface", .specVersion = VK_API_VERSION_1_0},
  };

  std::vector<char const*> required_extensions = {
    "VK_KHR_surface",
    "VK_KHR_missing_extension",
  };

  CHECK_FALSE(rf3d::hri::vk::verify_extensions_support(supported_extensions, required_extensions));
}

TEST_SUITE_END();
