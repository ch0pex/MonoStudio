
find_package(doctest CONFIG REQUIRED)
if (NOT EXISTS ${PROJECT_NAME}-tests)
  add_custom_target(${PROJECT_NAME}-tests)
endif()

function(create_test test_name test_src)
  add_executable(test_${PROJECT_NAME}_${test_name} ${test_src})
  target_link_libraries(test_${PROJECT_NAME}_${test_name} PRIVATE doctest::doctest ${PROJECT_NAME}-lib)
  add_test(test_${PROJECT_NAME}_${test_name} test_${PROJECT_NAME}_${test_name})
  add_dependencies(${PROJECT_NAME}-tests test_${PROJECT_NAME}_${test_name})
  set_tests_properties(test_${PROJECT_NAME}_${test_name} PROPERTIES
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  )
endfunction()

function(integration_test test_name test_src)
  add_executable(ftest_${PROJECT_NAME}_${test_name} ${test_src})
  target_link_libraries(ftest_${PROJECT_NAME}_${test_name} PRIVATE ${PROJECT_NAME}-lib)
  add_dependencies(${PROJECT_NAME}-tests ftest_${PROJECT_NAME}_${test_name})
endfunction()

