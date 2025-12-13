cmake_minimum_required(VERSION 3.26)
project(MonoGames LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

if (BUILD_TESTING)
  enable_testing()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Engine
add_subdirectory(src/libs/rflect3d)

# Games
add_subdirectory(src/apps/vixel)
# add_subdirectory(src/apps/ezTrade)

