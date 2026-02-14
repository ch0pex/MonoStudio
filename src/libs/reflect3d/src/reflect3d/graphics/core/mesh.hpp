#pragma once

#include "reflect3d/graphics/core/vertex.hpp"

#include <vector>

namespace rf3d::gfx {

struct Mesh {
  std::vector<Vertex> vertices;
  std::vector<Index> indices;
};


} // namespace rf3d::gfx
