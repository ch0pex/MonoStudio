
# Input
create_test(input test_input.cpp)

# Graphics
create_test(vk_extensions graphics/vk/test_vk_extensions.cpp LABELS "graphics")
create_test(vk_validation_layers graphics/vk/test_vk_validation_layers.cpp LABELS "graphics")
create_test(shader_compiler graphics/shaders/test_shader_compiler.cpp)
target_copy_slang_binaries(test_reflect3d_shader_compiler)


# Window
create_test(window window/test_window.cpp LABELS "window")
create_test(window_tracker window/test_window_tracker.cpp LABELS "window")
create_test(window_builder window/test_window_builder.cpp LABELS "window")
create_test(window_modes window/test_window_modes.cpp LABELS "window")

