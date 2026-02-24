#include "common.hpp"

#include "mono/misc/as_span.hpp"
#include "reflect3d/graphics/rhi.hpp"
#include "reflect3d/input/input.hpp"

#include <mono/error/expected.hpp>
#include <mono/execution/signals.hpp>
#include <mono/execution/stop_token.hpp>
#include <mono/logging/logger.hpp>

#include <iostream>

using namespace rf3d;
using namespace rf3d::gfx;
using namespace rf3d::gfx::vk;

std::array constexpr vertices = {
  Vertex {.position = {-0.5F, -0.5F, 0.F}, .color {1.0F, 0.0F, 0.0F, 0.0F}},
  Vertex {.position = {0.5F, -0.5F, 0.F}, .color = {0.0F, 1.0F, 0.0F, 0.F}},
  Vertex {.position = {0.5F, 0.5F, 0.F}, .color = {0.0F, 0.0F, 1.0F, 0.F}},
  Vertex {.position = {-0.5F, 0.5F, 0.F}, .color = {1.0F, 1.0F, 1.0F, 0.F}}
};

int main() {

  Mesh mesh {
    .vertices = std::ranges::to<std::vector>(vertices),
    .indices  = std::vector<Index> {0, 1, 2, 2, 3, 0},
  };

  test::renderer<rf3d::impl::vk>(1, mono::as_span(mesh));
}
