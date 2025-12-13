add_library(mono-core-lib INTERFACE)

target_sources(
  mono-core-lib
  PUBLIC
    FILE_SET HEADERS
    FILES
      # Config
      mono-core/config/base_config.hpp
      mono-core/config/parser.hpp

      # Logging
      mono-core/logging/logger_config.hpp
      mono-core/logging/logger.hpp

      # Containers
      mono-core/containers/stable_vector.hpp
      mono-core/containers/static_vector.hpp

      # Misc
      mono-core/misc/passkey.hpp
      mono-core/misc/defaults.hpp

      # Error
      mono-core/error/log_error.hpp
      mono-core/error/expected.hpp

      # Execution
      mono-core/execution/signals.hpp
      mono-core/execution/stop_token.hpp

      # Program
      mono-core/program/options.hpp
      mono-core/program/handle_error.hpp
)

target_link_libraries(
  mono-core-lib
  INTERFACE
    boost::boost
    quill::quill
    STDEXEC::stdexec
    reflectcpp::reflectcpp
)

add_library(mono::core-lib ALIAS mono-core-lib)
