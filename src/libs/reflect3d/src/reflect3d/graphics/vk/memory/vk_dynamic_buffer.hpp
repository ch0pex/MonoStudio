#pragma once

#include "reflect3d/graphics/vk/memory/vk_buffer.hpp"


namespace rf3d::gfx::vk {

/**
 * @brief Generic host visble buffer
 *
 * DynamicBuffer is a generic buffer mapped to host memory, allowing for direct CPU access.
 * The buffer is created with specific allocation flags to optimize sequential write access
 *
 * @tparam Type The type of elements stored in the buffer. Must be trivially copyable.
 *
 * @note Read operations in mapped memory are slow, so it's recommended to avoid them if possible.
 */
template<typename Type>
  requires(std::is_trivially_copyable_v<Type>)
class DynamicBuffer : public Buffer<Type> {
public:
  /*********************
   *    Type traits    *
   *********************/

  using size_type          = std::size_t;
  using value_type         = Type;
  using mapped_memory_type = std::span<value_type>;
  using iterator           = mapped_memory_type::iterator;
  using const_iterator     = mapped_memory_type::const_iterator;

  /*********************
   *    Constructor    *
   *********************/

  explicit DynamicBuffer(core::BufferCreateInfo const& buffer_info) :
    Buffer<Type> {
      buffer_info,
      {
        .flags          = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
        .usage          = VMA_MEMORY_USAGE_AUTO_PREFER_HOST,
        .requiredFlags  = {},
        .preferredFlags = {},
        .memoryTypeBits = 0,
        .pool           = nullptr,
        .pUserData      = nullptr,
        .priority       = 0.0F,
      }
    },
    mapped_memory(
        static_cast<Type*>(this->allocation_info().pMappedData), this->allocation_info().size / sizeof(Type)
    ) { }

  /**************************
   *    Member functions    *
   **************************/

  template<typename Self>
  auto begin(this Self&& self) {
    return std::ranges::begin(std::forward<Self>(self).mapped_memory);
  }

  template<typename Self>
  auto end(this Self&& self) {
    return std::ranges::end(std::forward<Self>(self).mapped_memory);
  }

  void insert(size_type const index, Type const& element) {
    std::memcpy(std::addressof(mapped_memory[index]), &element, sizeof(Type));
  }

  void insert_range(size_type const index, std::ranges::contiguous_range auto elements) {
    std::ranges::copy(elements.begin(), elements.end(), mapped_memory.begin() + index);
  }

  // @note read operations in mapped memory are slow avoid them if possible
  value_type& operator[](size_type const index) { return mapped_memory[index]; }

  value_type const& operator[](size_type const index) const { return mapped_memory[index]; }

  value_type& at(size_type const index) { return mapped_memory.at(index); }

  value_type const& at(size_type const index) const { return mapped_memory.at(index); }

  mapped_memory_type data() const { return mapped_memory; }

private:
  std::span<Type> mapped_memory;
};

} // namespace rf3d::gfx::vk
