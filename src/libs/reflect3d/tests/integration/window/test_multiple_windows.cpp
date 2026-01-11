
#include "common.hpp"
#include "reflect3d/window/window_types.hpp"


int main(int argc, char** argv) { //
  using namespace rf3d;
  mono::ex::setup_signals();


  std::vector<Window> windows;
  windows.reserve(10);

  for (std::size_t i = 0; i < 3; ++i) {
    auto window_config = config::Window {.title = std::format("Window {}", i), .mode = WindowMode::windowed};

    auto win = rf3d::WindowBuilder(window_config).default_callbacks().build(); //
    windows.emplace_back(std::move(win));
  }

  while (mono::ex::should_run()) {
    input::poll_events();
    for (auto& window: windows) {
      window.make_context_current();
    }
  }
}
