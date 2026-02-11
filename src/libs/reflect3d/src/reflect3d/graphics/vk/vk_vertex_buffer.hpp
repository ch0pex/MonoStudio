#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/render/vertex.hpp"
#include "vk_buffer.hpp"

namespace rf3d::gfx::vk {
//
// template<typename T>
// class GpuBuffer {
//   using buffer_type = raii::Buffer;
//
// };
//
// using VertexBuffer = GpuBuffer<>;
//

class VertexBuffer {
public:
  using allocation_type = AllocatedBuffer;
  using buffer_type     = core::Buffer;
  using size_type       = std::size_t;

  explicit VertexBuffer(std::size_t const size) :
    buffer_handles(
        gpu::allocate_buffer(
            core::BufferCreateInfo {
              .size        = size * sizeof(Vertex),
              .usage       = core::BufferUsageFlagBits::eVertexBuffer,
              .sharingMode = core::SharingMode::eExclusive,
            },
            {
              .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
              .usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
              .requiredFlags  = {},
              .preferredFlags = {},
              .memoryTypeBits = 0,
              .pool           = nullptr,
              .pUserData      = nullptr,
              .priority       = 0.0F,
            }
        )
    ),
    mapped_memory(static_cast<Vertex*>(buffer_handles.allocation_info.pMappedData), size) { }

  ~VertexBuffer() { }

  Vertex& at(size_type const index) { return mapped_memory[index]; }

  template<typename Self>
  auto begin(this Self&& self) {
    return std::ranges::begin(self.mapped_memory);
  }

  template<typename Self>
  auto end(this Self&& self) {
    return std::ranges::end(self.mapped_memory);
  }

  void insert(size_type const index, Vertex const& vertex) { mapped_memory[index] = vertex; }

  void insert_range(size_type const index, std::span<Vertex const> vertices) {
    std::ranges::copy(vertices.begin(), vertices.end(), mapped_memory.begin() + index);
  }

  core::Buffer view() const { return buffer_handles.buffer; }

private:
  AllocatedBuffer buffer_handles;
  std::span<Vertex> mapped_memory {};
};


} // namespace rf3d::gfx::vk
