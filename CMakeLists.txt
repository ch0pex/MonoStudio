cmake_minimum_required(VERSION 3.26)
project(MonoGames LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if (BUILD_TESTING)
  enable_testing()
endif()

include(dependencies)

# Libs
add_subdirectory(src/libs/mono-core)
add_subdirectory(src/libs/reflect3d)

# Apps
add_subdirectory(src/apps/vixel)
# add_subdirectory(src/apps/ezTrade)

