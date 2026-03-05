#pragma once

#include <cstdint>

namespace rf3d::gfx {

enum class ResourceState : std::uint8_t {
  undefined = 0,
  vertex_buffer,
  constant_buffer,
  index_buffer,
  render_target,
  depth_write,
  depth_read,
  shader_resource,
  unordered_access,
  copy_source,
  copy_destination,
  indirect_argument,
  present
};

}
