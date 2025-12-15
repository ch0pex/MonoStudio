project(mono)

add_subdirectory(include)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
