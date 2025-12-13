add_library(vixel-lib INTERFACE)

target_sources(
  vixel-lib
  PUBLIC
    FILE_SET HEADERS
    FILES

    # Program
    vixel/program/main_window.hpp
    vixel/program/dispatcher.hpp
    vixel/program/config/config.hpp
    vixel/program/config/example.hpp

    # Game
    vixel/game/game.hpp
)

target_link_libraries(vixel-lib INTERFACE reflect3d-lib)

add_library(vixel::vixel-lib ALIAS vixel-lib)
