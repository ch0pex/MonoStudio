
# Containers
create_test(deferred_erase_hive test_deferred_hive.cpp)


# Thread
create_test(thread test_thread.cpp)

# Program
create_test(program_options test_program_options.cpp)
create_test(program_config test_program_config.cpp)
create_test(signals test_signals.cpp)

# Error
create_test(expected test_expected.cpp)

# Execution
create_test(execution_inspect test_execution_inspect.cpp)
create_test(execution_expect test_execution_expect.cpp)

# Misc
create_test(start_lifetime_as test_start_lifetime_as.cpp)
create_test(as_span test_as_span.cpp)

# Ranges
create_test(cycle_view test_cycle_view.cpp)
