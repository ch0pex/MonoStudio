add_library(reflect3d-lib INTERFACE)

target_sources(
  reflect3d-lib
  PUBLIC
    FILE_SET HEADERS
    FILES

      # Graphics
      reflect3d/graphics/hri.hpp
      reflect3d/graphics/vk/utils/vk_exception.hpp
      reflect3d/graphics/vk/utils/vk_result.hpp
      reflect3d/graphics/vk/utils/vk_checker.hpp
      reflect3d/graphics/vk/utils/vk_debug_callback.hpp
      reflect3d/graphics/vk/vk_debug_messenger.hpp
      reflect3d/graphics/vk/vk_instance.hpp
      reflect3d/graphics/vk/vk_instance_detail.hpp
      reflect3d/graphics/vk/vk_instance_profiles.hpp
      reflect3d/graphics/vk/vk_gpu.hpp
      reflect3d/graphics/vk/vk_physical_device.hpp
      reflect3d/graphics/vk/vk_physical_device_detail.hpp
      reflect3d/graphics/vk/vk_traits.hpp
      reflect3d/graphics/vk/vk_extensions.hpp
      reflect3d/graphics/vk/vk_validation_layers.hpp
      reflect3d/graphics/vk/vk_gpu_families.hpp
      reflect3d/graphics/vk/vk_gpu_families_detail.hpp
      reflect3d/graphics/vk/vk_swapchain.hpp
      reflect3d/graphics/vk/vk_swapchain_detail.hpp

      # Input
      reflect3d/input/input.hpp

      # Math
      reflect3d/math/math.hpp

      # Window
      reflect3d/window/window_tracker.hpp
      reflect3d/window/monitor.hpp
      reflect3d/window/window_modes.hpp
      reflect3d/window/window_types.hpp
      reflect3d/window/utils/callbacks.hpp
      reflect3d/window/utils/resolution.hpp
      reflect3d/window/utils/context.hpp
      reflect3d/window/utils/defaults.hpp
      reflect3d/window/utils/window_error.hpp
      reflect3d/window/window_builder.hpp
      reflect3d/window/window_config.hpp
      reflect3d/window/window.hpp

      # Render
      reflect3d/render/viewport.hpp
      reflect3d/render/vertex.hpp
      reflect3d/render/mesh.hpp
      reflect3d/render/renderer.hpp
)

target_link_libraries(
  reflect3d-lib
  INTERFACE
    glfw
    Vulkan::Loader
    glm::glm
    mono::mono-lib
    glad::glad
)

add_library(mono::reflect3d-lib ALIAS reflect3d-lib)

target_compile_definitions(reflect3d-lib INTERFACE
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)
