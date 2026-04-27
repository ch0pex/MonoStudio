#pragma once

#include "reflect3d/graphics/core/buffer_info.hpp"
#include "reflect3d/graphics/core/enum_flags.hpp"
#include "reflect3d/graphics/core/resource_state.hpp"

#include <cstdint>
#include <span>
#include <type_traits>

#include "mono/meta/concepts.hpp"

namespace rf3d::archetypes {

struct Any { };

/**
 * Buffer archetype that models a GPU buffer resource.
 *
 * Since C++ do not support universal quantification, this mock type
 * can be used to by concepts to trick this language limitation.
 */
template<typename T, BufferUsage Usage, MemoryProperty Mem = MemoryProperty::dedicated>
struct Buffer {
  using size_type    = std::size_t;
  using handle_type  = std::uintptr_t;
  using state_type   = ResourceState;
  using usage        = std::integral_constant<BufferUsage, Usage>;
  using memory       = std::integral_constant<MemoryProperty, Mem>;
  using host_visible = std::bool_constant<detail::contains_flag(Mem, MemoryProperty::mapped)>;

  // Buffer(config_type const& config);

  Buffer(Buffer const&) = delete;

  Buffer(Buffer&&) = default;

  Buffer& operator=(Buffer const&) = delete;

  Buffer& operator=(Buffer&&) = default;

  ~Buffer() = default;

  [[nodiscard]] std::size_t size() const;

  // [[nodiscard]] std::size_t capacity() const;

  [[nodiscard]] handle_type handle() const;

  template<mono::meta::trivially_copyable_value U>
  [[nodiscard]] mono::span<U> mapped_data() const
    requires(host_visible::value);

  [[nodiscard]] state_type current_state() const
    requires(not host_visible::value);

  void set_state(state_type state) const
    requires(not host_visible::value);
};

using DedicatedBuffer   = Buffer<Any, BufferUsage::destination, MemoryProperty::dedicated>;
using MappedBuffer      = Buffer<Any, BufferUsage::source, MemoryProperty::mapped>;
using DestinationBuffer = Buffer<Any, BufferUsage::destination>;
using SourceBuffer      = Buffer<Any, BufferUsage::source>;
using UniformBuffer     = Buffer<Any, BufferUsage::uniform, MemoryProperty::mapped>;
using StorageBuffer     = Buffer<Any, BufferUsage::storage>;
using VertexBuffer      = Buffer<Any, BufferUsage::vertex>;
using IndexBuffer       = Buffer<Any, BufferUsage::index>;

} // namespace rf3d::archetypes
