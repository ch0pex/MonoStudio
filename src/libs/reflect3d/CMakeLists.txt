project(reflect3d)

include(assets)

add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()
