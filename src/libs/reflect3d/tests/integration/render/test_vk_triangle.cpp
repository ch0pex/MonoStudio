

#include "reflect3d/graphics/vk/vk_barriers.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window_builder.hpp"

#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>

#include <iostream>


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

  auto const& queues = gpu.queues();


  while (mono::ex::should_run()) {
    rf3d::input::poll_events();

    queues.graphics().waitIdle(); // For simplicity

    auto [index, image]        = swapchain.next_image();
    auto const& command_buffer = gpu.command_buffer(0);

    // --- Begin Record commands ---
    {
      command_buffer.begin({});
      transition_image_layout(command_buffer, image, gfx::vk::rendering_layout);

      gfx::vk::core::ClearValue clearColor                   = gfx::vk::core::ClearColorValue(0.0F, 0.0F, 0.0F, 1.0F);
      gfx::vk::core::RenderingAttachmentInfo attachment_info = {
        .imageView   = image.view(),
        .imageLayout = gfx::vk::core::ImageLayout::eColorAttachmentOptimal,
        .loadOp      = gfx::vk::core::AttachmentLoadOp::eClear,
        .storeOp     = gfx::vk::core::AttachmentStoreOp::eStore,
        .clearValue  = clearColor
      };
      auto extent                                 = swapchain.extent();
      gfx::vk::core::RenderingInfo rendering_info = {
        .renderArea           = {.offset = {.x = 0, .y = 0}, .extent = extent},
        .layerCount           = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments    = &attachment_info
      };

      command_buffer.beginRendering(rendering_info);

      command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *gpu.pipeline());
      command_buffer.setViewport(
          0, vk::Viewport(0.0F, 0.0F, static_cast<float>(extent.width), static_cast<float>(extent.height), 0.0F, 1.0F)
      );
      command_buffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), extent));
      command_buffer.setCullMode(gfx::vk::core::CullModeFlagBits::eBack);
      command_buffer.setFrontFace(gfx::vk::core::FrontFace::eClockwise);
      command_buffer.setPrimitiveTopology(gfx::vk::core::PrimitiveTopology::eTriangleList);
      command_buffer.setLineWidth(1.0F);
      command_buffer.draw(3, 1, 0, 0);

      command_buffer.endRendering();
      transition_image_layout(command_buffer, image, gfx::vk::present_layout);
      command_buffer.end();
    }
    // --- End Record commands ---

    gpu.reset_fences(swapchain.render_fence());

    vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    vk::SubmitInfo const submitInfo {
      .waitSemaphoreCount   = 1,
      .pWaitSemaphores      = &*swapchain.present_semaphore(),
      .pWaitDstStageMask    = &waitDestinationStageMask,
      .commandBufferCount   = 1,
      .pCommandBuffers      = &*command_buffer,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores    = &*swapchain.render_semaphore()
    };

    queues.graphics().submit(submitInfo, *swapchain.render_fence());
    gpu.wait_fences(swapchain.render_fence());

    vk::PresentInfoKHR const presentInfoKHR {
      .waitSemaphoreCount = 1,
      .pWaitSemaphores    = &*swapchain.render_semaphore(),
      .swapchainCount     = 1,
      .pSwapchains        = &**swapchain,
      .pImageIndices      = &index
    };

    auto result = queues.graphics().presentKHR(presentInfoKHR);
    switch (result) {
      case vk::Result::eSuccess:
        break;
      case vk::Result::eSuboptimalKHR:
        std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
        break;
      default:
        break; // an unexpected result is returned!
    }
  }

  gpu.queues().graphics().waitIdle();
}
catch (std::exception const& e) {
  std::cerr << "Unhandled exception: " << e.what() << '\n';
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unhandled unknown exception\n";
  return EXIT_FAILURE;
}
