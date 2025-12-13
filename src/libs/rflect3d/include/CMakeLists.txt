find_package(glfw3 CONFIG REQUIRED)
find_package(VulkanLoader CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED)
find_package(quill CONFIG REQUIRED)
find_package(reflectcpp CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
include(stdexec)

add_library(rflect3d-lib INTERFACE)

target_sources(
  rflect3d-lib
  PUBLIC
    FILE_SET HEADERS
    FILES
      # Config
      rflect3d/config/parser.hpp

      # Core
      rflect3d/core/logging/logger_config.hpp
      rflect3d/core/logging/logger.hpp
      rflect3d/core/containers/stable_vector.hpp
      rflect3d/core/containers/static_vector.hpp
      rflect3d/core/misc/passkey.hpp
      rflect3d/core/misc/defaults.hpp
      rflect3d/core/error/log_error.hpp
      rflect3d/core/error/expected.hpp
      rflect3d/core/execution/signals.hpp
      rflect3d/core/execution/stop_token.hpp

      # Program
      rflect3d/program/options.hpp
      rflect3d/program/handle_error.hpp

      # Input
      rflect3d/input/input.hpp

      # Math
      rflect3d/math/math.hpp

      # Window
      rflect3d/window/window_tracker.hpp
      rflect3d/window/utils/callbacks.hpp
      rflect3d/window/utils/resolution.hpp
      rflect3d/window/utils/context.hpp
      rflect3d/window/utils/defaults.hpp
      rflect3d/window/window_builder.hpp
      rflect3d/window/window_config.hpp
      rflect3d/window/window.hpp
      rflect3d/window/window_handle.hpp

      # Render
      rflect3d/render/viewport.hpp
      rflect3d/render/vertex.hpp
      rflect3d/render/mesh.hpp
      rflect3d/render/renderer.hpp
)

target_link_libraries(
  rflect3d-lib
  INTERFACE
    glfw
    Vulkan::Loader
    boost::boost
    quill::quill
    reflectcpp::reflectcpp
    STDEXEC::stdexec
    glm::glm
)

# add_library(rflect3d::rflect3d-lib ALIAS rflect3d-lib)
