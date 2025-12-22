# Input
create_test(input test_input.cpp)

# Graphics
create_test(vk_instance graphics/vk/test_vk_instance.cpp LABELS "graphics")


# Window
create_test(window window/test_window.cpp LABELS "window")
create_test(window_tracker window/test_window_tracker.cpp LABELS "window")
create_test(window_builder window/test_window_builder.cpp LABELS "window")
create_test(window_modes window/test_window_modes.cpp LABELS "window")

