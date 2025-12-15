add_library(mono-lib INTERFACE)

target_sources(
  mono-lib
  PUBLIC
    FILE_SET HEADERS
    FILES
      # Config
      mono/config/base_config.hpp
      mono/config/parser.hpp

      # Logging
      mono/logging/logger_config.hpp
      mono/logging/logger.hpp

      # Containers
      mono/containers/stable_vector.hpp
      mono/containers/static_vector.hpp

      # Misc
      mono/misc/passkey.hpp
      mono/misc/defaults.hpp

      # Error
      mono/error/log_error.hpp
      mono/error/expected.hpp

      # Execution
      mono/execution/signals.hpp
      mono/execution/stop_token.hpp

      # Program
      mono/program/options.hpp
      mono/program/handle_error.hpp
)

target_link_libraries(
  mono-lib
  INTERFACE
    boost::boost
    quill::quill
    STDEXEC::stdexec
    reflectcpp::reflectcpp
)

add_library(mono::core-lib ALIAS mono-lib)
