
find_package(doctest CONFIG REQUIRED)
if (NOT EXISTS ${PROJECT_NAME}-tests)
  add_custom_target(${PROJECT_NAME}-tests)
endif()

function(create_test test_name test_src)
  set(options "")
  set(oneValueArgs RESOURCE_LOCK)
  set(multiValueArgs LABELS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(test_${PROJECT_NAME}_${test_name} ${test_src})
  target_link_libraries(test_${PROJECT_NAME}_${test_name} PRIVATE doctest::doctest ${PROJECT_NAME}-lib)

  add_test(NAME test_${PROJECT_NAME}_${test_name}
           COMMAND test_${PROJECT_NAME}_${test_name})

  add_dependencies(${PROJECT_NAME}-tests test_${PROJECT_NAME}_${test_name})

  if(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set_tests_properties(test_${PROJECT_NAME}_${test_name} PROPERTIES
        WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
  endif()

  set(final_labels "${PROJECT_NAME}")

  if(ARG_LABELS)
    list(APPEND final_labels ${ARG_LABELS})
  endif()

  set_tests_properties(test_${PROJECT_NAME}_${test_name} PROPERTIES
      LABELS "${final_labels}"
  )
  if(ARG_RESOURCE_LOCK)
    set_tests_properties(test_${PROJECT_NAME}_${test_name} PROPERTIES
        RESOURCE_LOCK "${ARG_RESOURCE_LOCK}"
    )

  endif()

endfunction()

function(integration_test test_name test_src)
  add_executable(ftest_${PROJECT_NAME}_${test_name} ${test_src})
  target_link_libraries(ftest_${PROJECT_NAME}_${test_name} PRIVATE ${PROJECT_NAME}-lib)
  add_dependencies(${PROJECT_NAME}-tests ftest_${PROJECT_NAME}_${test_name})
endfunction()

