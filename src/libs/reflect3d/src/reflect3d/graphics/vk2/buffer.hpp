#pragma once

#include "mono/meta/concepts.hpp"
#include "mono/misc/passkey.hpp"
#include "mono/misc/start_lifetime_as.hpp"
#include "reflect3d/graphics/api/command_list.hpp"
#include "reflect3d/graphics/core2/buffer_info.hpp"
#include "reflect3d/graphics/core2/enum_flags.hpp"
#include "reflect3d/graphics/core2/resource_state.hpp"
#include "reflect3d/graphics/vk2/detail/memory/vk_buffer_allocation.hpp"
#include "reflect3d/graphics/vk2/detail/vk_gpu_detail.hpp"

#include <cstddef>
#include <type_traits>

namespace rf3d::vk {

/**
 * Buffer class represents a GPU buffer resource that can be used for various
 * purposes such as vertex buffers, index buffers,
 *
 * @tparam T The type of the elements stored in the buffer. Must be trivially copyable.
 * @tparam Usage The intended usage of the buffer, specified as a combination of BufferUsage
 */
template<BufferUsage Usage, MemoryProperty Mem = MemoryProperty::dedicated>
class Buffer {
public:
  // --- Type traits ---

  using size_type    = std::size_t;
  using handle_type  = detail::core::Buffer;
  using state_type   = ResourceState;
  using usage        = std::integral_constant<BufferUsage, Usage>;
  using memory       = std::integral_constant<MemoryProperty, Mem>;
  using host_visible = std::bool_constant<rf3d::detail::contains_flag(Mem, MemoryProperty::mapped)>;

  // --- Constructors ---

  template<mono::meta::trivially_copyable_value T>
  explicit Buffer(BufferInfo<T> const& config) :
    buffer_allocation(detail::allocate_buffer({}, {})), //
    element_count(config.capacity) //,
  // name(std::format("Buffer<{}>-{}", to_string(usage::value)))  //
  { }

  Buffer(Buffer&& other) noexcept :
    buffer_allocation(std::exchange(other.buffer_allocation, {})),
    element_count(std::exchange(other.element_count, 0)) { }

  Buffer& operator=(Buffer&& other) noexcept {
    if (this != &other) {

      if (buffer_allocation.buffer_handle != nullptr) {
        detail::free_buffer(buffer_allocation);
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
      detail::free_buffer(buffer_allocation);
    }
  }

  // --- Member functions ---

  [[nodiscard]] std::size_t size() const { return element_count; }

  [[nodiscard]] std::size_t size_bytes() const { return buffer_allocation.allocation_info.size; }

  [[nodiscard]] handle_type handle() const { return buffer_allocation.buffer_handle; }

protected:
  [[nodiscard]] detail::BufferAllocation const& allocation() const { return buffer_allocation; }

private:
  detail::BufferAllocation buffer_allocation;
  size_type element_count;
  std::string name {"TODO"};
};


/**
 * MappedBuffer are those buffers that are directly accessible by the CPU.
 * They are typically used for uniform buffers, staging buffers, and other
 * resources that require frequent updates from the CPU.
 *
 * @tparam T The type of the elements stored in the buffer.
 * @tparam Usage The intended usage of the buffer, specified as a combination of BufferUsage
 */
template<BufferUsage Usage>
class MappedBuffer : Buffer<Usage, MemoryProperty::mapped> {
public:
  using size_type    = typename Buffer<Usage, MemoryProperty::dedicated>::size_type;
  using handle_type  = typename Buffer<Usage, MemoryProperty::dedicated>::handle_type;
  using state_type   = typename Buffer<Usage, MemoryProperty::dedicated>::state_type;
  using usage        = typename Buffer<Usage, MemoryProperty::dedicated>::usage;
  using memory       = typename Buffer<Usage, MemoryProperty::dedicated>::memory;
  using host_visible = typename Buffer<Usage, MemoryProperty::dedicated>::host_visible;

  // --- Constructors ---

  template<mono::meta::trivially_copyable_value T>
  explicit MappedBuffer(BufferInfo<T> const& config) : Buffer<Usage, MemoryProperty::mapped>(config) {
    assert(this->allocation().allocation_info.size > 0 && "Mapped memory size must be greater than zero");
    assert(this->allocation().allocation_handle.pMappedData != nullptr && "Mapped memory pointer must not be null");

    auto mapped_data = this->template mapped_data<T>();
    std::ranges::copy(config.data, mapped_data.begin());
  }

  // --- Member functions ---
  template<typename T>
    requires std::is_trivially_copyable_v<T>
  [[nodiscard]] mono::span<T const> mapped_data() const {
    std::size_t const size = this->allocation().allocation_info.size / sizeof(T);
    T* ptr                 = mono::start_lifetime_as_array<T>(this->allocation().allocation_handle.pMappedData, size);
    return {ptr, size};
  }
};

/*
 * DedicatedBuffer are those buffers that are not directly accessible by the CPU.
 * They are typically used for vertex buffers, index buffers, and other GPU-only resources.
 *
 * @tparam T The type of the elements stored in the buffer. Must be trivially copyable.
 * @tparam Usage The intended usage of the buffer, specified as a combination of BufferUsage
 */
template<BufferUsage Usage>
class DedicatedBuffer : public Buffer<Usage, MemoryProperty::dedicated> {
public:
  // --- Type traits ---
  using size_type    = typename Buffer<Usage, MemoryProperty::dedicated>::size_type;
  using handle_type  = typename Buffer<Usage, MemoryProperty::dedicated>::handle_type;
  using state_type   = typename Buffer<Usage, MemoryProperty::dedicated>::state_type;
  using usage        = typename Buffer<Usage, MemoryProperty::dedicated>::usage;
  using memory       = typename Buffer<Usage, MemoryProperty::dedicated>::memory;
  using host_visible = typename Buffer<Usage, MemoryProperty::dedicated>::host_visible;

  // --- Constructors ---
  template<mono::meta::trivially_copyable_value T>
  explicit DedicatedBuffer(BufferInfo<T> const& config) : Buffer<Usage, MemoryProperty::dedicated>(config) {
    // CopyCommandList cmd_list {};
    //
    // cmd_list.begin()
    //     .copy_buffer(*this, config.data, config.capacity)
    //     .end();
  }

  template<std::ranges::contiguous_range Range>
    requires mono::meta::trivially_copyable_value<std::ranges::range_value_t<Range>>
  explicit DedicatedBuffer(Range&& data) :
    Buffer<Usage, MemoryProperty::dedicated>(BufferInfo<std::ranges::range_value_t<Range>> {
      .data = std::forward<Range>(data), .capacity = std::ranges::size(data)
    }) { }

  // --- Member functions ---
  [[nodiscard]] state_type current_state() const { return state; }

  /**
   * Only command lists are allowed to change the state of the buffer,
   * because they are responsible for synchronizing access to the buffer
   * and ensuring that the correct barriers are in place when transitioning between states.
   *
   * @tparam T the type of the command list that is changing the state of the texture
   */
  template<rf3d::CopyCommandList CmdList>
  void set_state(state_type const new_state, mono::PassKey<CmdList> /*key*/) {
    state = new_state;
  }

private:
  ResourceState state {ResourceState::undefined};
};

// --- Type aliases ---

using VertexStorageBuffer = DedicatedBuffer<BufferUsage::vertex | BufferUsage::storage | BufferUsage::destination>;
using VertexBuffer        = DedicatedBuffer<BufferUsage::vertex | BufferUsage::destination>;
using IndexBuffer         = DedicatedBuffer<BufferUsage::index | BufferUsage::destination>;
using TransferBuffer      = MappedBuffer<BufferUsage::source>;
using UniformBuffer       = MappedBuffer<BufferUsage::uniform | BufferUsage::destination>;
using StorageBuffer       = DedicatedBuffer<BufferUsage::storage | BufferUsage::destination>;
}; // namespace rf3d::vk
