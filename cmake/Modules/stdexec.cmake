include(CPM)

set(STDEXEC_BUILD_TESTS OFF)
set(STDEXEC_ENABLE_TESTING OFF)
set(STDEXEC_BUILD_EXAMPLES OFF)
set(STDEXEC_ENABLE_IO_URING ON)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(STDEXEC_BUILD_TESTS OFF CACHE BOOL "Disable stdexec tests" FORCE)
set(STDEXEC_BUILD_EXAMPLES OFF CACHE BOOL "Disable stdexec examples" FORCE)

CPMAddPackage(
        NAME stdexec
        GITHUB_REPOSITORY NVIDIA/stdexec
        GIT_TAG 3ccbe56d646db55c7f94c2467f012a9647d6fa23
        SYSTEM YES
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(stdexec INTERFACE -Wno-shadow -Wno-old-style-cast -Wno-sign-conversion -Wno-format=2)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(stdexec INTERFACE -Wno-shadow -Wno-old-style-cast -Wno-sign-conversion -Wno-format)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(stdexec INTERFACE /wd4702) # disable unreachable code warning
endif ()

