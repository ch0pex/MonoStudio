include(testing)

# Thread
create_test(test_thread core/test_thread.cpp)

# Program
create_test(test_program_options program/test_program_options.cpp)
create_test(test_program_config program/test_program_config.cpp)
create_test(test_signals program/test_signals.cpp)

# Error
create_test(test_expected core/test_expected.cpp)

# Input
create_test(test_input test_input.cpp)

# Window
create_test(test_window window/test_window.cpp)
create_test(test_window_tracker window/test_window_tracker.cpp)
create_test(test_window_builder window/test_window_builder.cpp)


