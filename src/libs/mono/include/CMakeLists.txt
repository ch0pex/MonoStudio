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

        # Meta
        mono/meta/concepts.hpp
        mono/meta/detail/specialization_of.hpp
        mono/meta/detail/template_parameter_of.hpp

        # Misc
        mono/misc/passkey.hpp
        mono/misc/defaults.hpp

        # Error
        mono/error/log_error.hpp
        mono/error/expected.hpp

        # Execution
        mono/execution/signals.hpp
        mono/execution/stop_token.hpp
        mono/execution/expect.hpp
        mono/execution/execution.hpp
        mono/execution/expect.hpp

        # Program
        mono/program/options.hpp
        mono/program/handle_error.hpp
)

#target_sources(mono-lib
#        PRIVATE
#        # Config
#        #        mono/logging/logger.cpp
#)

target_link_libraries(
        mono-lib
        INTERFACE
        boost::boost
        quill::quill
        STDEXEC::stdexec
        reflectcpp::reflectcpp
        plf_colony::plf_colony
)

add_library(mono::mono-lib ALIAS mono-lib)

target_compile_definitions(mono-lib INTERFACE QUILL_DISABLE_NON_PREFIXED_MACROS)

