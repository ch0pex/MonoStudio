#include "reflect3d/graphics/vk/utils/vk_defaults.hpp"
#include "reflect3d/graphics/vk/vk_barriers.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/graphics/vk/vk_instance.hpp"
#include "reflect3d/graphics/vk/vk_swapchain.hpp"
#include "reflect3d/input/input.hpp"
#include "reflect3d/window/window_builder.hpp"
#include "reflect3d/window/window_config.hpp"

#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>

#include <iostream>
#include <vulkan/vulkan_structs.hpp>

int main() try {
  using namespace rf3d;
  mono::ex::setup_signals();

  gfx::vk::Instance instance {};
  gfx::vk::Gpu gpu = instance.create_gpu();

  std::vector<Window> windows;
  gfx::vk::SwapchainsManager swapchains;

  for (std::size_t i = 0; i < 3; ++i) {
    auto window_config = config::Window {
      .title = std::format("Window {}", i), .mode = WindowMode::windowed, .monitor = static_cast<MonitorId>(i)
    };

    auto win = rf3d::WindowBuilder(window_config).default_callbacks().build(); //

    gfx::vk::Swapchain sc = gpu.create_swapchain(
        win.create_surface(instance), //
        win.resolution() //
    );

    swapchains.emplace_back(std::move(sc));
    windows.emplace_back(std::move(win));
  }

  while (mono::ex::should_run()) {
    rf3d::input::poll_events();

    auto const [frame_index, command_buffer] = gpu.command_buffer();

    command_buffer.record({}, [&swapchains, &gpu, frame_index](gfx::vk::CommandBuffer const& cmd) {
      for (auto& swapchain: swapchains) {
        auto const& image = swapchain.next_image(frame_index);

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
      }
    });

    auto const present_semaphores = swapchains.present_semaphores(frame_index);
    auto const render_semaphores  = swapchains.render_semaphores();
    auto const swapchain_handles  = swapchains.swapchain_handles();
    auto const masks              = swapchains.wait_destination_stage_masks();
    auto const indices            = swapchains.image_indices();

    vk::SubmitInfo const submit_info {
      .waitSemaphoreCount   = static_cast<std::uint32_t>(present_semaphores.size()),
      .pWaitSemaphores      = present_semaphores.data(),
      .pWaitDstStageMask    = masks.data(),
      .commandBufferCount   = 1,
      .pCommandBuffers      = &**command_buffer,
      .signalSemaphoreCount = static_cast<std::uint32_t>(render_semaphores.size()),
      .pSignalSemaphores    = render_semaphores.data()
    };

    gpu.submit_work<gfx::vk::GraphicQueue>(submit_info);

    gfx::vk::core::PresentInfoKHR const present_info {
      .waitSemaphoreCount = static_cast<std::uint32_t>(render_semaphores.size()),
      .pWaitSemaphores    = render_semaphores.data(),
      .swapchainCount     = static_cast<std::uint32_t>(swapchain_handles.size()),
      .pSwapchains        = swapchain_handles.data(),
      .pImageIndices      = indices.data(),
    };

    gpu.present(present_info);
  }
  gpu.wait_idle();
}
catch (std::exception const& e) {
  std::cerr << "Unhandled exception: " << e.what() << '\n';
  return EXIT_FAILURE;
}
catch (...) {
  std::cerr << "Unhandled unknown exception\n";
  return EXIT_FAILURE;
}
