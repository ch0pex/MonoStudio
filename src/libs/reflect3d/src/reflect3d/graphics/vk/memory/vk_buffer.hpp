#pragma once

#include "reflect3d/graphics/vk/vk_gpu.hpp"

#include <mono/meta/concepts.hpp>

namespace rf3d::gfx::vk {

template<mono::meta::trivially_copyable_value Type>
class Buffer {
public:
  /*********************
   *    Type traits    *
   *********************/

  using handle_type          = core::Buffer;
  using allocation_info_type = AllocationInfo;


  /**********************
   *    Constructors    *
   **********************/

  explicit Buffer(core::BufferCreateInfo const& buffer_info, AllocationCreateInfo const& alloc_info) :
    buffer_allocation(gpu::allocate_buffer(buffer_info, alloc_info)) {
    if (buffer_info.size % sizeof(Type) != 0) {
      throw std::runtime_error("Buffer size must be a multiple of the element type size");
    }
  }

  Buffer(Buffer&&) = default;

  Buffer& operator=(Buffer const&) = delete;

  Buffer& operator=(Buffer&&) = default;

  Buffer(Buffer const&) = delete;

  ~Buffer() { gpu::free_buffer(buffer_allocation); }

  /**************************
   *    Member functions    *
   **************************/

  [[nodiscard]] handle_type handle() const { return buffer_allocation.buffer_handle; }

  [[nodiscard]] allocation_info_type allocation_info() const { return buffer_allocation.allocation_info; }

private:
  BufferAllocation buffer_allocation;
};

} // namespace rf3d::gfx::vk
