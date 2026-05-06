#pragma once

#include "reflect3d/graphics/api/buffer.hpp"
#include "reflect3d/graphics/api/command_list.hpp"
#include "reflect3d/graphics/core/buffer_info.hpp"
#include "reflect3d/graphics/core/enum_flags.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"
#include "reflect3d/graphics/vk/command_list.hpp"
#include "reflect3d/graphics/vk/detail/memory/vk_buffer_allocation.hpp"
#include "reflect3d/graphics/vk/detail/utils/vk_to_native.hpp"
#include "reflect3d/graphics/vk/detail/vk_gpu_detail.hpp"

#include <mono/meta/concepts.hpp>
#include <mono/misc/passkey.hpp>
#include <mono/misc/start_lifetime_as.hpp>

#include <cstddef>
#include <ranges>
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

  // @note: size specified is the garanteed minimum size of the buffer,
  // but the actual allocated size might be greater due to alignment requirements and padding added by the allocator.
  explicit Buffer(size_type const size) :
    buffer_allocation(
        detail::allocate_buffer(
            {
              .size  = size,
              .usage = detail::to_native<usage::value>(),
            },
            Mem == MemoryProperty::mapped //
                ? detail::mapped_allocation_create_info
                : detail::device_local_allocation_create_info
        )
    ) //
  // name(std::format("Buffer<{}>-{}", to_string(usage::value)))  //
  { }

  Buffer(Buffer&& other) noexcept : buffer_allocation(std::exchange(other.buffer_allocation, {})) { }

  Buffer& operator=(Buffer&& other) noexcept {
    if (this != &other) {

      if (buffer_allocation.buffer_handle != nullptr) {
        detail::free_buffer(buffer_allocation);
      }

      buffer_allocation = std::exchange(other.buffer_allocation, {});
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

  // This size might not be the same as the size specified during buffer creation, because of alignment requirements and
  // padding added by the allocator. However, it will always be greater than or equal to the requested size.
  [[nodiscard]] std::size_t size() const { return buffer_allocation.allocation_info.size; }

  [[nodiscard]] std::size_t size_bytes() const { return buffer_allocation.allocation_info.size; }

  [[nodiscard]] handle_type handle() const { return buffer_allocation.buffer_handle; }

protected:
  [[nodiscard]] detail::BufferAllocation const& allocation() const { return buffer_allocation; }

private:
  detail::BufferAllocation buffer_allocation;
  // std::string name {}; TODO: add debug names to buffers
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
class MappedBuffer : public Buffer<Usage, MemoryProperty::mapped> {
public:
  using size_type    = Buffer<Usage, MemoryProperty::mapped>::size_type;
  using handle_type  = Buffer<Usage, MemoryProperty::mapped>::handle_type;
  using state_type   = Buffer<Usage, MemoryProperty::mapped>::state_type;
  using usage        = Buffer<Usage, MemoryProperty::mapped>::usage;
  using memory       = Buffer<Usage, MemoryProperty::mapped>::memory;
  using host_visible = Buffer<Usage, MemoryProperty::mapped>::host_visible;

  // --- Constructors ---

  explicit MappedBuffer(size_type const size) : Buffer<Usage, MemoryProperty::mapped>(size) {
    assert(this->allocation().allocation_info.size > 0 and "Mapped memory size must be greater than zero");
    assert(this->allocation().allocation_info.pMappedData != nullptr and "Mapped memory pointer must not be null");
  }

  // --- Member functions ---
  template<mono::meta::trivially_copyable_value T>
  [[nodiscard]] mono::span<T> mapped_data() const {
    std::size_t const size = this->allocation().allocation_info.size / sizeof(T);
    T* ptr                 = mono::start_lifetime_as_array<T>(this->allocation().allocation_info.pMappedData, size);
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
class DedicatedBuffer : public Buffer<Usage> {
public:
  // --- Type traits ---
  using size_type    = Buffer<Usage>::size_type;
  using handle_type  = Buffer<Usage>::handle_type;
  using state_type   = Buffer<Usage>::state_type;
  using usage        = Buffer<Usage>::usage;
  using memory       = Buffer<Usage>::memory;
  using host_visible = Buffer<Usage>::host_visible;

  // --- Constructors ---
  // @note: size specified is the garanteed minimum size of the buffer,
  // but the actual allocated size might be greater due to alignment requirements and padding added by the allocator.
  explicit DedicatedBuffer(size_type const size) : Buffer<Usage, MemoryProperty::dedicated>(size) { }

  // --- Member functions ---
  [[nodiscard]] state_type current_state() const { return state; }

  /**
   * Only command lists are allowed to change the state of the buffer,
   * because they are responsible for synchronizing access to the buffer
   * and ensuring that the correct barriers are in place when transitioning between states.
   *
   * @tparam CmdList the type of the command list that is changing the state of the texture
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
