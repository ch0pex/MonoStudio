#pragma once

#include "reflect3d/graphics/vk/detail/utils/vk_native_types.hpp"
#include "reflect3d/graphics/vk/detail/vk_gpu.hpp"

#include <mono/logging/logger.hpp>
#include <mono/meta/concepts.hpp>

namespace rf3d::vk::detail {

template<mono::meta::trivially_copyable_value Type>
class Buffer {
public:
  /*********************
   *    Type traits    *
   *********************/

  using handle_type          = core::Buffer;
  using allocation_info_type = AllocationInfo;
  using size_type            = std::size_t;


  /**********************
   *    Constructors    *
   **********************/

  explicit Buffer(core::BufferCreateInfo const& buffer_info, AllocationCreateInfo const& alloc_info) :
    buffer_allocation(gpu::allocate_buffer(buffer_info, alloc_info)), //
    element_count(buffer_info.size / sizeof(Type)), //
    name(std::format("Buffer<{}>-{}", to_string(buffer_info.usage), counter++))
  //
  {
    if (buffer_info.size % sizeof(Type) != 0) {
      throw std::runtime_error("Buffer size must be a multiple of the element type size");
    }
    LOG_INFO("Created {} with size {} bytes", name, buffer_allocation.allocation_info.size);
  }

  Buffer(Buffer&& other) noexcept :
    buffer_allocation(std::exchange(other.buffer_allocation, {})),
    element_count(std::exchange(other.element_count, 0)) { }

  Buffer& operator=(Buffer&& other) noexcept {
    if (this != &other) {

      if (buffer_allocation.buffer_handle != nullptr) {
        gpu::free_buffer(buffer_allocation);
      }

      buffer_allocation = std::exchange(other.buffer_allocation, {});
      element_count     = std::exchange(other.element_count, 0);
    }
    return *this;
  }

  Buffer(Buffer const&) = delete;

  Buffer& operator=(Buffer const&) = delete;

  ~Buffer() {
    if (buffer_allocation.buffer_handle != nullptr) {
      // LOG_INFO("Destroying {} with size {} bytes", name, buffer_allocation.allocation_info.size);
      gpu::free_buffer(buffer_allocation);
    }
  }

  /**************************
   *    Member functions    *
   **************************/

  [[nodiscard]] handle_type handle() const { return buffer_allocation.buffer_handle; }

  [[nodiscard]] allocation_info_type allocation_info() const { return buffer_allocation.allocation_info; }

  [[nodiscard]] std::size_t size() const { return element_count; }

  [[nodiscard]] std::size_t size_bytes() const { return buffer_allocation.allocation_info.size; }


private:
  BufferAllocation buffer_allocation;
  size_type element_count;
  std::string name;
  inline static auto counter = 0;
};

} // namespace rf3d::vk::detail
