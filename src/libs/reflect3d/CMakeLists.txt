project(reflect3d)


add_subdirectory(include)

include(slang)
add_slang_shader_target(reflect3d-shaders SOURCES ${CMAKE_CURRENT_LIST_DIR}/assets/shaders/shader.slang)
add_dependencies(reflect3d-lib reflect3d-shaders)


if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
