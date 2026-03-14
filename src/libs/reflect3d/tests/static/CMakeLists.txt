
add_library(reflect3d-lib-static-tests STATIC
  graphics_api.cpp
  vk_to_native.cpp
)

target_link_libraries(reflect3d-lib-static-tests PRIVATE mono::reflect3d-lib)
