/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file vertex.hpp
 * @version 1.0
 * @date 06/04/2024
 * @brief Vertex
 *
 * mesh vertex definition
 *
 */

#pragma once

#include <reflect3d/math/math.hpp>

namespace rf3d::gfx {

struct Vertex {
  math::vec3 pos {0.0F, 0.0F, 0.0F};
  math::vec4 color {0.4F, 0.4F, 0.4F, 0.0F};
  math::vec3 normal {0.0F, 0.0F, 0.0F};
  math::vec2 uv {0.0F, 0.0F};
};

} // namespace rf3d::gfx
