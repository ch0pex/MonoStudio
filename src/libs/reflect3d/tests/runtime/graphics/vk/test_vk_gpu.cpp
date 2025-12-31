#include <reflect3d/graphics/vk/vk_gpu.hpp>
#include <reflect3d/graphics/vk/vk_instance.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_SUITE_BEGIN("Vulkan");

TEST_CASE("Create instance and gpu") {
  CHECK_NOTHROW({
    rf3d::gfx::vk::Instance instance {};
    rf3d::gfx::vk::Gpu gpu = instance.pick_gpu(rf3d::gfx::vk::best_gpu_criteria);
  });
}

TEST_SUITE_END();
