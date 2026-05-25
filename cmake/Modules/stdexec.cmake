include(CPM)

set(STDEXEC_BUILD_TESTS OFF)
set(STDEXEC_ENABLE_TESTING OFF)
set(STDEXEC_BUILD_EXAMPLES OFF)
set(STDEXEC_ENABLE_IO_URING ON)

set(STDEXEC_BUILD_TESTS OFF CACHE BOOL "Disable stdexec tests" FORCE)
set(STDEXEC_BUILD_EXAMPLES OFF CACHE BOOL "Disable stdexec examples" FORCE)

CPMAddPackage(
        NAME stdexec
        GITHUB_REPOSITORY NVIDIA/stdexec
        GIT_TAG feff2aacf1d1f8470bce31442487fa557d7fdd76
        SYSTEM YES
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(stdexec INTERFACE -Wno-shadow -Wno-old-style-cast -Wno-sign-conversion -Wno-format=2)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(stdexec INTERFACE -Wno-shadow -Wno-old-style-cast -Wno-sign-conversion -Wno-format)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(stdexec INTERFACE /wd4702 /wd4714) # disable unreachable code warning and force inline warning
endif ()

