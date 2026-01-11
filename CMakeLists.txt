cmake_minimum_required(VERSION 3.30)

message("---------------- ${CMAKE_CXX_COMPILER_IMPORT_STD} ------------------")

add_compile_definitions(MONO_USE_CPP_MODULES=1)

project(MonoGames LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(dependencies)
include(testing)

# Libs
add_subdirectory(src/libs/mono)
add_subdirectory(src/libs/reflect3d)

# Apps
add_subdirectory(src/apps/vixel)
add_subdirectory(src/apps/feed-viewer)
# add_subdirectory(src/apps/ezTrade)

