# Input
create_test(input test_input.cpp)

# Window
create_test(window window/test_window.cpp LABELS "graphics")
create_test(window_tracker window/test_window_tracker.cpp LABELS "graphics")
create_test(window_builder window/test_window_builder.cpp LABELS "graphics")
create_test(window_modes window/test_window_modes.cpp LABELS "graphics")

