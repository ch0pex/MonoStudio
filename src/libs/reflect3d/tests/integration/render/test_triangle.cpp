#include "common.hpp"

#include "reflect3d/graphics/hri.hpp"
#include "reflect3d/input/input.hpp"

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>

#include <iostream>

using namespace rf3d;
using namespace rf3d::gfx;
using namespace rf3d::gfx::vk;

inline constexpr Vertex vertex1 {.position = {0.0F, -0.5F, 0.0F}, .color = {1.0F, 0.0F, 0.0F, 1.0F}};
inline constexpr Vertex vertex2 {.position = {0.5F, 0.5F, 0.0F}, .color = {0.0F, 1.0F, 0.0F, 1.0F}};
inline constexpr Vertex vertex3 {.position = {-0.5F, 0.5F, 0.0F}, .color = {0.0F, 0.0F, 1.0F, 1.0F}};
inline constexpr std::array vertices = {vertex1, vertex2, vertex3};

template<typename hri>
void test_square() try {
  using namespace rf3d;
  using namespace rf3d::gfx;


  mono::ex::setup_signals();
  LOG_INFO("Begining of the program");


  auto surface = test::create_test_surface<hri>("Vk Triangle");
  typename hri::renderer gfx {};

  gfx.add_mesh(
      Mesh {
        .vertices = std::ranges::to<std::vector>(vertices),
        .indices  = std::vector<Index> {0, 1, 2, 2, 3, 0},
      }
  );

  while (mono::ex::should_run()) {
    input::poll_events();
    gfx.render_surface(surface, {});
  }
}
catch (std::exception const& e) {
  std::cerr << "Unhandled exception: " << e.what() << '\n';
}
catch (...) {
  std::cerr << "Unhandled unknown exception\n";
}


int main() { test_square<rf3d::gfx::Vulkan>(); }
