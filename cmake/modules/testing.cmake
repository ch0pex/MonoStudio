
find_package(doctest CONFIG REQUIRED)
add_custom_target(${PROJECT_NAME}-tests)

function(create_test test_name test_src)
  add_executable(${test_name} ${test_src})
  target_link_libraries(${test_name} PRIVATE doctest::doctest ${PROJECT_NAME}-lib)
  add_test(test_${PROJECT_NAME}_${test_name} ${test_name})
  add_dependencies(${PROJECT_NAME}-tests ${test_name})
endfunction()

