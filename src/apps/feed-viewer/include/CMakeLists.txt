add_library(feed-viewer-lib INTERFACE)

target_sources(
  feed-viewer-lib
  PUBLIC
    FILE_SET HEADERS
    FILES
    feed-viewer/feed-viewer.hpp
    feed-viewer/program/dispatcher.hpp
    feed-viewer/program/config.hpp
)

target_link_libraries(feed-viewer-lib INTERFACE mono::mono-lib)

add_library(mono::feed-viewer-lib ALIAS feed-viewer-lib)
