macro(add_target_assets_path TARGET_NAME)
  set(ASSETS_ABSOLUTE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets")

  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/src/assets_path.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/assets_path.hpp"
)
  target_include_directories(${TARGET_NAME} INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/generated")
endmacro()
