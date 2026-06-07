macro(add_target_assets_path TARGET_NAME ASSETS_PATH)
  set(ASSETS_ABSOLUTE_PATH "${ASSETS_PATH}")

  configure_file(
    "${CMAKE_SOURCE_DIR}/cmake/src/assets_path.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/assets_path.hpp"
)
  # TODO check if is interface target
  target_include_directories(${TARGET_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/generated")
endmacro()
