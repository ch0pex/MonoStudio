#pragma once

#include "reflect3d/graphics/vk/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/vk_gpu.hpp"
#include "reflect3d/render/vertex.hpp"

namespace rf3d::gfx::vk {
//
// template<typename T>
// class GpuBuffer {
//   using buffer_type = raii::Buffer;
//
//
//
// };
//
// using VertexBuffer = GpuBuffer<>;
//


class VertexBuffer {
public:
  using memory_type = raii::DeviceMemory;
  using buffer_type = raii::Buffer;
  using size_type   = std::size_t;

  explicit VertexBuffer(std::size_t const size) :
    buffer(
        gpu::make_buffer({
          .size        = sizeof(Vertex) * size,
          .usage       = core::BufferUsageFlagBits::eVertexBuffer,
          .sharingMode = core::SharingMode::eExclusive,
        })
    ),
    memory(
        gpu::allocate_memory(
            buffer.getMemoryRequirements(),
            core::MemoryPropertyFlagBits::eHostVisible | core::MemoryPropertyFlagBits::eHostCoherent
        )
    ) {
    buffer.bindMemory(*memory, 0);
    void* data    = memory.mapMemory(0, size * sizeof(Vertex));
    mapped_memory = std::span(static_cast<Vertex*>(data), size);
  }

  ~VertexBuffer() { memory.unmapMemory(); }

  Vertex& at(size_type const index) { return mapped_memory[index]; }

  template<typename Self>
  auto begin(Self&& self) {
    return std::ranges::begin(self.mapped_memory);
  }

  template<typename Self>
  auto end(Self&& self) {
    return std::ranges::end(self.mapped_memory);
  }

  void insert(size_type const index, Vertex const& vertex) { mapped_memory[index] = vertex; }

  void insert_range(size_type const index, std::span<Vertex const> vertices) {
    std::ranges::copy(vertices.begin(), vertices.end(), mapped_memory.begin() + index);
  }

  core::Buffer view() const { return *buffer; }

private:
  buffer_type buffer;
  memory_type memory;
  std::span<Vertex> mapped_memory {};
};


} // namespace rf3d::gfx::vk
