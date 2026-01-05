project(reflect3d)

include(slang)
include(assets)

add_subdirectory(include)

add_slang_shader_target(reflect3d-shaders SOURCES ${CMAKE_CURRENT_LIST_DIR}/assets/shaders/shader.slang)
add_dependencies(reflect3d-lib reflect3d-shaders)

add_target_assets_path(reflect3d-lib)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
