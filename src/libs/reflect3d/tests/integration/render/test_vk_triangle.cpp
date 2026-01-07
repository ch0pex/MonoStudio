

#include "reflect3d/graphics/vk/utils/vk_checker.hpp"
#include "reflect3d/graphics/vk/vk_barriers.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"
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


  while (mono::ex::should_run()) {
    rf3d::input::poll_events();

    auto [frame_index, command_buffer] = gpu.command_buffer();
    auto [image_index, image]          = swapchain.next_image(frame_index);

    command_buffer.begin({}); // --- Begin Record commands --- TODO use lambda instead of begin/end
    {
      transition_image_layout(command_buffer, image, gfx::vk::rendering_layout);

      gfx::vk::core::ClearValue const clear_color = gfx::vk::core::ClearColorValue {0.0F, 0.0F, 0.0F, 1.0F};
      gfx::vk::core::RenderingAttachmentInfo const attachment_info = {
        .imageView   = image.view(),
        .imageLayout = gfx::vk::core::ImageLayout::eColorAttachmentOptimal,
        .loadOp      = gfx::vk::core::AttachmentLoadOp::eClear,
        .storeOp     = gfx::vk::core::AttachmentStoreOp::eStore,
        .clearValue  = clear_color
      };
      auto extent                                 = swapchain.extent();
      gfx::vk::core::RenderingInfo rendering_info = {
        .renderArea           = {.offset = {.x = 0, .y = 0}, .extent = extent},
        .layerCount           = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments    = &attachment_info
      };

      command_buffer.beginRendering(rendering_info); //  TODO use lambda instead of begin/end

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
    }
    command_buffer.end(); // --- End Record commands ---

    vk::PipelineStageFlags wait_destination_stage_mask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    vk::SubmitInfo const submitInfo {
      .waitSemaphoreCount   = 1,
      .pWaitSemaphores      = &*swapchain.present_semaphore(frame_index),
      .pWaitDstStageMask    = &wait_destination_stage_mask,
      .commandBufferCount   = 1,
      .pCommandBuffers      = &*command_buffer,
      .signalSemaphoreCount = 1,
      .pSignalSemaphores    = &*swapchain.render_semaphore(image_index)
    };

    gpu.submit_work<gfx::vk::GraphicQueue>(submitInfo);

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
