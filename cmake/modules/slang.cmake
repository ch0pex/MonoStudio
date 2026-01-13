#############################################
# Information

# This fetches a precompiled release of Slang, and makes it available through:
#  - Target 'slang' links to slang dynamic library.
#  - Variable 'SLANGC' points to the slangc executable.
#  - Variable 'SLANG_LIB_FOUND' is ON iff we could find Slang library that
#    matches the target system
#  - Variable 'SLANG_EXE_FOUND' is ON iff we could find Slang executable that
#    matches the host system
#  - Function 'target_copy_slang_binaries' should be called for all executable
#    target the indirectly links slang so that the dll/dylib/so file of slang
#    is copied next to the generated binary.

# NB: This file is compatible with cross-compilation setups. In such a case,
# executable is fetched for the host system while library is fetched for the
# target system.

#############################################
# Options

include(FetchContent)
set(SLANG_VERSION "2024.14.4" CACHE STRING "Version of the Slang release to use, without the leading 'v'. Must correspond to a release available at https://github.com/shader-slang/slang/releases (or whatever the SLANG_MIRROR variable is set to).")
set(SLANG_MIRROR "https://github.com/shader-slang/slang" CACHE STRING "This is the source from which release binaries are fetched, set by default to official Slang repository but you may change it to use a fork.")

#############################################
# Target architecture detection (thank you CMake for not providing that...)
# (NB: Partly copied from third_party/webgpu/cmake/utils.cmake)
function(detect_arch target_or_host out_var)
    if (target_or_host STREQUAL "TARGET")
        set(SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(IS_64BIT ON)
        elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(IS_64BIT OFF)
        endif ()
    elseif (target_or_host STREQUAL "HOST")
        set(SYSTEM_PROCESSOR ${CMAKE_HOST_SYSTEM_PROCESSOR})
        cmake_host_system_information(RESULT IS_64BIT QUERY IS_64BIT)
    else ()
        message(FATAL_ERROR "First argument of detect_arch() must be either TARGET or HOST")
    endif ()

    if (SYSTEM_PROCESSOR STREQUAL "AMD64" OR SYSTEM_PROCESSOR STREQUAL "x86_64" OR SYSTEM_PROCESSOR STREQUAL "x86")
        if (IS_64BIT)
            set(ARCH "x86_64")
        else ()
            set(ARCH "i686")
        endif ()
    elseif (SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|armv8|arm)$")
        set(ARCH "aarch64")
    elseif (SYSTEM_PROCESSOR MATCHES "^(armv7|armv6|armhf)$")
        set(ARCH "arm")
    else ()
        message(WARNING "Unknown architecture: ${SYSTEM_PROCESSOR}")
        set(ARCH "unknown")
    endif ()
    set(${out_var} ${ARCH} PARENT_SCOPE)
endfunction(detect_arch)

#############################################
# Build URL to fetch
# NB: The Slang binaries might be different for the library and executable,
# in case of cross-compilation.

set(URL_OS)
set(SLANG_LIB_FOUND ON)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(URL_OS "windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(URL_OS "linux")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(URL_OS "macos")
else ()
    message(STATUS "Target system '${CMAKE_SYSTEM_NAME}' not supported by this release of Slang. Disabling slang library target.")
    set(SLANG_LIB_FOUND OFF)
endif ()
if (SLANG_LIB_FOUND)
    detect_arch(TARGET ARCH)
    set(URL_NAME "slang-${SLANG_VERSION}-${URL_OS}-${ARCH}")
    string(TOLOWER "${URL_NAME}" FC_NAME)
    set(URL "${SLANG_MIRROR}/releases/download/v${SLANG_VERSION}/${URL_NAME}.zip")
endif (SLANG_LIB_FOUND)

set(HOST_URL_OS)
set(SLANG_EXE_FOUND ON)
if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(HOST_URL_OS "windows")
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(HOST_URL_OS "linux")
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(HOST_URL_OS "macos")
else ()
    message(STATUS "Host system '${CMAKE_HOST_SYSTEM_NAME}' not supported by this release of Slang. Disabling slang executable target.")
    set(SLANG_EXE_FOUND OFF)
endif ()
if (SLANG_EXE_FOUND)
    detect_arch(HOST HOST_ARCH)
    set(URL_NAME "slang-${SLANG_VERSION}-${HOST_URL_OS}-${HOST_ARCH}")
    string(TOLOWER "${URL_NAME}" HOST_FC_NAME)
    set(HOST_URL "${SLANG_MIRROR}/releases/download/v${SLANG_VERSION}/${URL_NAME}.zip")
endif (SLANG_EXE_FOUND)

#############################################
# Declare FetchContent, then make available

if (SLANG_LIB_FOUND)
    FetchContent_Declare(${FC_NAME} URL ${URL})
    message(STATUS "Using Slang binaries from '${URL}'")
    FetchContent_MakeAvailable(${FC_NAME})
    set(Slang_ROOT "${${FC_NAME}_SOURCE_DIR}")
endif (SLANG_LIB_FOUND)

if (SLANG_EXE_FOUND AND NOT HOST_URL STREQUAL URL)
    FetchContent_Declare(${HOST_FC_NAME} URL ${HOST_URL})
    message(STATUS "Using Slang HOST binaries from '${HOST_URL}'")
    FetchContent_MakeAvailable(${HOST_FC_NAME})
    set(Slang_HOST_ROOT "${${HOST_FC_NAME}_SOURCE_DIR}")
else ()
    set(Slang_HOST_ROOT ${Slang_ROOT})
endif ()

#############################################
# Import targets (ideally slang would provide a SlangConfig.cmake)

if (SLANG_LIB_FOUND)

    add_library(slang SHARED IMPORTED GLOBAL)

    # 1. Definimos el mapeo de configuraciones (Coverage/Sanitize -> Release)
    set_target_properties(slang PROPERTIES
            MAP_IMPORTED_CONFIG_COVERAGE "Release"
            MAP_IMPORTED_CONFIG_SANITIZE "Release"
            MAP_IMPORTED_CONFIG_RELWITHDEBINFO "Release"
            MAP_IMPORTED_CONFIG_DEBUG "Release"
    )

    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        if (MSVC)
            # FIX: Definimos explícitamente IMPORTED_LOCATION_RELEASE y IMPORTED_IMPLIB_RELEASE
            set_target_properties(
                    slang
                    PROPERTIES
                    IMPORTED_IMPLIB "${Slang_ROOT}/lib/slang.lib"
                    IMPORTED_IMPLIB_RELEASE "${Slang_ROOT}/lib/slang.lib"
                    IMPORTED_IMPLIB_DEBUG "${Slang_ROOT}/lib/slang.lib"

                    IMPORTED_LOCATION "${Slang_ROOT}/bin/slang.dll"
                    IMPORTED_LOCATION_RELEASE "${Slang_ROOT}/bin/slang.dll"
                    IMPORTED_LOCATION_DEBUG "${Slang_ROOT}/bin/slang.dll"
            )
        else ()
            message(FATAL_ERROR "Sorry, Slang does not provide precompiled binaries for MSYS/MinGW")
        endif ()
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        # FIX: Hacemos lo mismo para Linux (.so)
        set_target_properties(
                slang
                PROPERTIES
                IMPORTED_LOCATION "${Slang_ROOT}/lib/libslang.so"
                IMPORTED_LOCATION_RELEASE "${Slang_ROOT}/lib/libslang.so"
                IMPORTED_LOCATION_DEBUG "${Slang_ROOT}/lib/libslang.so"
                IMPORTED_NO_SONAME TRUE
        )
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        # FIX: Y para macOS (.dylib)
        set_target_properties(
                slang
                PROPERTIES
                IMPORTED_LOCATION "${Slang_ROOT}/lib/libslang.dylib"
                IMPORTED_LOCATION_RELEASE "${Slang_ROOT}/lib/libslang.dylib"
                IMPORTED_LOCATION_DEBUG "${Slang_ROOT}/lib/libslang.dylib"
        )
    endif ()

    target_include_directories(slang INTERFACE
            "${Slang_ROOT}/include"
    )

endif (SLANG_LIB_FOUND)

#############################################
# Set output variables

SET(SLANG_LIB_FOUND "${SLANG_LIB_FOUND}" CACHE BOOL "On if Slang executable was found for the target system.")
SET(SLANG_EXE_FOUND "${SLANG_EXE_FOUND}" CACHE BOOL "On if Slang executable was found for the host system.")

if (SLANG_EXE_FOUND)

    if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        set(EXE_EXT ".exe")
    else ()
        set(EXE_EXT)
    endif ()
    SET(SLANGC "${Slang_HOST_ROOT}/bin/slangc${EXE_EXT}" CACHE PATH "Path to the slangc executable, set by FetchSlang.cmake")

endif (SLANG_EXE_FOUND)

#############################################
# Utility function

if (SLANG_LIB_FOUND)

    # Get path to .dll/.so/.dylib, for target_copy_slang_binaries
    get_target_property(SLANG_RUNTIME_LIB slang IMPORTED_LOCATION)
    message(STATUS "Using Slang runtime '${SLANG_RUNTIME_LIB}'")
    set(SLANG_RUNTIME_LIB ${SLANG_RUNTIME_LIB} CACHE INTERNAL "Path to the Slang library binary")

    # The application's binary must find the .dll/.so/.dylib at runtime,
    # so we automatically copy it next to the binary.
    function(target_copy_slang_binaries TargetName)
        add_custom_command(
                TARGET ${TargetName} POST_BUILD
                COMMAND
                ${CMAKE_COMMAND} -E copy_if_different
                ${SLANG_RUNTIME_LIB}
                $<TARGET_FILE_DIR:${TargetName}>
                COMMENT
                "Copying '${SLANG_RUNTIME_LIB}' to '$<TARGET_FILE_DIR:${TargetName}>'..."
        )
    endfunction(target_copy_slang_binaries)

endif (SLANG_LIB_FOUND)

function(add_slang_shader_target TARGET)
    cmake_parse_arguments("SHADER" "" "" "SOURCES" ${ARGN})
    set(SHADERS_DIR ${CMAKE_CURRENT_LIST_DIR}/assets/shaders)
    set(ENTRY_POINTS -entry vertMain -entry fragMain)

    if (WIN32)
        set(SLANG_COMPILER "slangc.exe")
    else ()
        set(SLANG_COMPILER "slangc")
    endif ()

    add_custom_command(
            OUTPUT ${SHADERS_DIR}/bin/${TARGET}.spv
            COMMAND ${CMAKE_COMMAND} -E make_directory ${SHADERS_DIR}/bin
            COMMAND ${SLANG_COMPILER} ${SHADER_SOURCES} -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name ${ENTRY_POINTS} -o ${SHADERS_DIR}/bin/${TARGET}.spv

            WORKING_DIRECTORY ${SHADERS_DIR}
            DEPENDS ${SHADER_SOURCES}
            COMMENT "Compiling Slang Shaders (${TARGET})"
            VERBATIM
    )
    add_custom_target(${TARGET} DEPENDS ${SHADERS_DIR}/bin/${TARGET}.spv)
endfunction()
