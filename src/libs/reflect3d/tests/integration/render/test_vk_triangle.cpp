

#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/vk_barriers.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window_builder.hpp"

#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>

#include <iostream>
#include <vulkan/vulkan_structs.hpp>


template<typename PlatformTraits>
struct GraphicsInterface {
  // surface.render()
  // surface.create()
  // surface.resize()
  // light.create()
  // light.update()
  // element.create()
  // element.update()
};


int main() try {
  using namespace rf3d;
  mono::ex::setup_signals();

  gfx::vk::Instance instance {};
  gfx::vk::Gpu gpu = instance.create_gpu();

  auto window = rf3d::WindowBuilder() //
                    .default_callbacks() // //
                    .build(); //

  gfx::vk::Swapchain swapchain = gpu.create_swapchain(
      window.create_surface(instance), //
      window.resolution() //
  );

  while (mono::ex::should_run()) {
    rf3d::input::poll_events();

    auto const [frame_index, command_buffer] = gpu.command_buffer();
    auto const [image_index, image]          = swapchain.next_image(frame_index);

    command_buffer.record({}, [&image, &swapchain, &gpu](gfx::vk::CommandBuffer const& cmd) {
      std::vector const attachment_info = {
        gfx::vk::defaults::attachament_info(image.view(), gfx::vk::defaults::clear_color)
      };

      transition_image_layout(cmd, image, gfx::vk::rendering_layout);

      auto const render_area    = gfx::vk::defaults::render_area(swapchain.resolution());
      auto const rendering_info = gfx::vk::defaults::rendering_info(render_area, attachment_info);
      cmd.record_rendering(rendering_info, [&gpu, &swapchain, &render_area](gfx::vk::CommandBuffer const& cmd) {
        cmd.bind_pipeline(vk::PipelineBindPoint::eGraphics, *gpu.pipeline())
            .set_viewport(0, gfx::vk::defaults::viewport(swapchain.resolution()))
            .set_scissor(0, render_area)
            .set_cull_mode(gfx::vk::core::CullModeFlagBits::eBack)
            .set_front_face(gfx::vk::core::FrontFace::eClockwise)
            .set_primitive_topology(gfx::vk::core::PrimitiveTopology::eTriangleList)
            .set_line_width(1.0F)
            .draw(3, 1, 0, 0);
      });

      transition_image_layout(cmd, image, gfx::vk::present_layout);
    });

    vk::PipelineStageFlags const wait_destination_stage_mask {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::SubmitInfo const submit_info {
      .waitSemaphoreCount   = 1,
      .pWaitSemaphores      = &*swapchain.present_semaphore(frame_index),
      .pWaitDstStageMask    = &wait_destination_stage_mask,
      .commandBufferCount   = 1,
      .pCommandBuffers      = &**command_buffer,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores    = &*swapchain.render_semaphore(image_index)
    };

    gpu.submit_work<gfx::vk::GraphicQueue>(submit_info);

    gfx::vk::core::PresentInfoKHR const present_info {
      .waitSemaphoreCount = 1,
      .pWaitSemaphores    = &*swapchain.render_semaphore(image_index),
      .swapchainCount     = 1,
      .pSwapchains        = &**swapchain,
      .pImageIndices      = &image_index
    };

    gpu.present(present_info);
  }
}
catch (std::exception const& e) {
  std::cerr << "Unhandled exception: " << e.what() << '\n';
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unhandled unknown exception\n";
  return EXIT_FAILURE;
}
