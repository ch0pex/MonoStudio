include(testing)

# Thread
create_test(thread core/test_thread.cpp)

# Program
create_test(program_options program/test_program_options.cpp)
create_test(program_config program/test_program_config.cpp)
create_test(signals program/test_signals.cpp)

# Error
create_test(expected core/test_expected.cpp)

# Input
create_test(input test_input.cpp)

# Window
create_test(window window/test_window.cpp)
create_test(window_tracker window/test_window_tracker.cpp)
create_test(window_builder window/test_window_builder.cpp)


