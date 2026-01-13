project(reflect3d)

include(assets)

add_subdirectory(include)
add_target_assets_path(reflect3d-lib)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()
