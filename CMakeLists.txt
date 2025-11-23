cmake_minimum_required(VERSION 3.26)
project(MonoGames LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

if (BUILD_TESTING)
  enable_testing()
endif()

# Engine
add_subdirectory(src/engine)

# Games
add_subdirectory(src/games/vixel)

