add_library(reflect3d-lib STATIC)

target_sources(
        reflect3d-lib
        PUBLIC
        FILE_SET HEADERS
        FILES

        # Graphics
        reflect3d/graphics/backends.hpp
        reflect3d/graphics/concepts.hpp
        reflect3d/graphics/rhi.hpp
        reflect3d/graphics/rhi2.hpp
        reflect3d/graphics/surface.hpp

        reflect3d/graphics/api/archetypes/buffer.hpp
        reflect3d/graphics/api/archetypes/command_list.hpp
        reflect3d/graphics/api/archetypes/fence.hpp
        reflect3d/graphics/api/archetypes/gpu.hpp
        reflect3d/graphics/api/archetypes/pso.hpp
        reflect3d/graphics/api/archetypes/surface.hpp
        reflect3d/graphics/api/archetypes/texture.hpp

        reflect3d/graphics/api/buffer.hpp
        reflect3d/graphics/api/command_list.hpp
        reflect3d/graphics/api/fence.hpp
        reflect3d/graphics/api/gpu.hpp
        reflect3d/graphics/api/pso.hpp
        reflect3d/graphics/api/surface.hpp
        reflect3d/graphics/api/texture.hpp

        reflect3d/graphics/core/frame_info.hpp
        reflect3d/graphics/core/mesh.hpp
        reflect3d/graphics/core/vertex.hpp

        reflect3d/graphics/core/shader/compiler.hpp
        reflect3d/graphics/core/shader/global_session.hpp
        reflect3d/graphics/core/shader/target_session.hpp
        reflect3d/graphics/core/shader/targets.hpp
        reflect3d/graphics/core/shader/types.hpp

        reflect3d/graphics/core2/buffer_info.hpp
        reflect3d/graphics/core2/command_list_type.hpp
        reflect3d/graphics/core2/defaults.hpp
        reflect3d/graphics/core2/enum_flags.hpp
        reflect3d/graphics/core2/pso_states.hpp
        reflect3d/graphics/core2/primitive_types.hpp
        reflect3d/graphics/core2/resource_state.hpp
        reflect3d/graphics/core2/texture_info.hpp
        reflect3d/graphics/core2/viewport.hpp

        reflect3d/graphics/core2/shader/compiler.hpp
        reflect3d/graphics/core2/shader/global_session.hpp
        reflect3d/graphics/core2/shader/target_session.hpp
        reflect3d/graphics/core2/shader/targets.hpp
        reflect3d/graphics/core2/shader/types.hpp

        reflect3d/graphics/vk/gpu/vk_command_buffer.hpp
        reflect3d/graphics/vk/gpu/vk_command_pool.hpp
        reflect3d/graphics/vk/gpu/vk_gpu_families.hpp
        reflect3d/graphics/vk/gpu/vk_gpu_families_detail.hpp
        reflect3d/graphics/vk/gpu/vk_gpu_queues.hpp
        reflect3d/graphics/vk/gpu/vk_logical_device.hpp
        reflect3d/graphics/vk/gpu/vk_physical_device.hpp
        reflect3d/graphics/vk/gpu/vk_submit_info.hpp

        reflect3d/graphics/vk/instance/vk_debug_messenger.hpp
        reflect3d/graphics/vk/instance/vk_extensions.hpp
        reflect3d/graphics/vk/instance/vk_instance_profiles.hpp
        reflect3d/graphics/vk/instance/vk_validation_layers.hpp

        reflect3d/graphics/vk/memory/vk_buffer.hpp
        reflect3d/graphics/vk/memory/vk_buffer_allocation.hpp
        reflect3d/graphics/vk/memory/vk_dynamic_buffer.hpp
        reflect3d/graphics/vk/memory/vk_index_buffer.hpp
        reflect3d/graphics/vk/memory/vk_memory_allocator.hpp
        reflect3d/graphics/vk/memory/vk_staging_buffer.hpp
        reflect3d/graphics/vk/memory/vk_static_buffer.hpp
        reflect3d/graphics/vk/memory/vk_vertex_buffer.hpp

        reflect3d/graphics/vk/utils/vk_checker.hpp
        reflect3d/graphics/vk/utils/vk_debug_callback.hpp
        reflect3d/graphics/vk/utils/vk_defaults.hpp
        reflect3d/graphics/vk/utils/vk_exception.hpp
        reflect3d/graphics/vk/utils/vk_native_types.hpp
        reflect3d/graphics/vk/utils/vk_result.hpp

        reflect3d/graphics/vk/vk_barriers.hpp
        reflect3d/graphics/vk/vk_gpu.hpp
        reflect3d/graphics/vk/vk_image.hpp
        reflect3d/graphics/vk/vk_instance.hpp
        reflect3d/graphics/vk/vk_pso.hpp
        reflect3d/graphics/vk/vk_pso_builder.hpp
        reflect3d/graphics/vk/vk_pso_manager.hpp
        reflect3d/graphics/vk/vk_renderer.hpp
        reflect3d/graphics/vk/vk_shader.hpp
        reflect3d/graphics/vk/vk_surface.hpp
        reflect3d/graphics/vk/vk_surface_info.hpp
        reflect3d/graphics/vk/vk_swapchain.hpp
        reflect3d/graphics/vk/vk_swapchain_detail.hpp

        reflect3d/graphics/vk2/buffer.hpp
        reflect3d/graphics/vk2/command_list.hpp
        reflect3d/graphics/vk2/fence.hpp
        reflect3d/graphics/vk2/fwd.hpp
        reflect3d/graphics/vk2/gpu.hpp
        reflect3d/graphics/vk2/pso.hpp
        reflect3d/graphics/vk2/semaphore.hpp
        reflect3d/graphics/vk2/surface.hpp
        reflect3d/graphics/vk2/texture.hpp

        reflect3d/graphics/vk2/detail/vk_barriers.hpp
        reflect3d/graphics/vk2/detail/vk_gpu_detail.hpp
        reflect3d/graphics/vk2/detail/vk_instance.hpp
        reflect3d/graphics/vk2/detail/vk_pso_builder.hpp
        reflect3d/graphics/vk2/detail/vk_pso_manager.hpp
        reflect3d/graphics/vk2/detail/vk_shader.hpp
        reflect3d/graphics/vk2/detail/vk_surface_info.hpp
        reflect3d/graphics/vk2/detail/vk_swapchain.hpp
        reflect3d/graphics/vk2/detail/vk_swapchain_detail.hpp

        reflect3d/graphics/vk2/detail/gpu/vk_command_pool.hpp
        reflect3d/graphics/vk2/detail/gpu/vk_gpu_families.hpp
        reflect3d/graphics/vk2/detail/gpu/vk_gpu_families_detail.hpp
        reflect3d/graphics/vk2/detail/gpu/vk_gpu_queues.hpp
        reflect3d/graphics/vk2/detail/gpu/vk_logical_device.hpp
        reflect3d/graphics/vk2/detail/gpu/vk_physical_device.hpp
        reflect3d/graphics/vk2/detail/gpu/vk_submit_info.hpp

        reflect3d/graphics/vk2/detail/instance/vk_debug_messenger.hpp
        reflect3d/graphics/vk2/detail/instance/vk_extensions.hpp
        reflect3d/graphics/vk2/detail/instance/vk_instance_profiles.hpp
        reflect3d/graphics/vk2/detail/instance/vk_validation_layers.hpp

        reflect3d/graphics/vk2/detail/memory/vk_buffer.hpp
        reflect3d/graphics/vk2/detail/memory/vk_buffer_allocation.hpp
        reflect3d/graphics/vk2/detail/memory/vk_dynamic_buffer.hpp
        reflect3d/graphics/vk2/detail/memory/vk_index_buffer.hpp
        reflect3d/graphics/vk2/detail/memory/vk_memory_allocator.hpp
        reflect3d/graphics/vk2/detail/memory/vk_staging_buffer.hpp
        reflect3d/graphics/vk2/detail/memory/vk_static_buffer.hpp
        reflect3d/graphics/vk2/detail/memory/vk_vertex_buffer.hpp

        reflect3d/graphics/vk2/detail/utils/vk_checker.hpp
        reflect3d/graphics/vk2/detail/utils/vk_debug_callback.hpp
        reflect3d/graphics/vk2/detail/utils/vk_defaults.hpp
        reflect3d/graphics/vk2/detail/utils/vk_exception.hpp
        reflect3d/graphics/vk2/detail/utils/vk_native_types.hpp
        reflect3d/graphics/vk2/detail/utils/vk_result.hpp
        reflect3d/graphics/vk2/detail/utils/vk_to_native.hpp

        # Input
        reflect3d/input/input.hpp

        # Math
        reflect3d/math/math.hpp

        # Window
        reflect3d/window/utils/callbacks.hpp
        reflect3d/window/utils/context.hpp
        reflect3d/window/utils/defaults.hpp
        reflect3d/window/utils/resolution.hpp
        reflect3d/window/utils/resolution_fmt.hpp
        reflect3d/window/utils/window_error.hpp
        reflect3d/window/monitor.hpp
        reflect3d/window/window.hpp
        reflect3d/window/window_builder.hpp
        reflect3d/window/window_config.hpp
        reflect3d/window/window_modes.hpp
        reflect3d/window/window_types.hpp

        # Render
        reflect3d/render/renderer.hpp
        reflect3d/render/viewport.hpp
)

target_sources(reflect3d-lib
        PRIVATE
        # Graphics
        reflect3d/graphics/vk/vk_renderer.cpp
        reflect3d/graphics/vk/utils/vk_vma_usage.cpp
        reflect3d/graphics/vk/vk_gpu.cpp
        reflect3d/graphics/vk/vk_instance.cpp
        reflect3d/graphics/vk2/gpu.cpp
        reflect3d/graphics/vk2/detail/vk_gpu_detail.cpp
        reflect3d/graphics/vk2/detail/vk_instance.cpp
        reflect3d/graphics/vk2/detail/utils/vk_vma_usage.cpp

        # Window
        reflect3d/window/utils/context.cpp
)

target_link_libraries(
        reflect3d-lib
        PUBLIC
        glfw
        Vulkan::Loader
        glm::glm
        mono::mono-lib
        glad::glad
        slang
        GPUOpen::VulkanMemoryAllocator
)

add_library(mono::reflect3d-lib ALIAS reflect3d-lib)

target_compile_definitions(reflect3d-lib PUBLIC
        RF3D_VULKAN_DEFAULT
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
        VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
        "VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS"
)
