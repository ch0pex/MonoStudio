/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rhi_buffer.hpp
 * @date 13/04/2026
 * @brief Short description
 *
 * Longer description
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/rhi_types.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---


namespace rf3d::rhi {

template<MappedBuffer Buffer>
Buffer make_buffer(std::ranges::contiguous_range auto initial_data, std::size_t capacity = 0) {
  using range_value_t = std::ranges::range_value_t<decltype(initial_data)>;

  capacity = std::max(capacity, initial_data.size() * sizeof(range_value_t));

  Buffer buffer {capacity};
  auto mapped_data = buffer.template mapped_data<range_value_t>();
  std::ranges::copy(initial_data, std::ranges::begin(mapped_data));
  return buffer;
}

template<DedicatedBuffer Buffer, RenderHardwareInterface Rhi = impl::DefaultBackend>
Buffer make_buffer(std::ranges::contiguous_range auto initial_data, std::size_t capacity = 0) {
  using range_value_t = std::ranges::range_value_t<decltype(initial_data)>;

  auto const size_bytes = initial_data.size() * sizeof(range_value_t);
  capacity              = std::max(capacity, size_bytes);

  Buffer buffer {capacity};
  BufferCopyRegion const copy_region {.size = size_bytes};
  auto staging_buffer = make_buffer<transfer_buffer_t<Rhi>>(initial_data, capacity);

  graphics_command_list_t<Rhi> cmd {};
  cmd.begin().copy_buffer(staging_buffer, buffer, copy_region).end();

  Rhi::gpu::submit_work(submit_info_t<Rhi> {.command_buffers = {cmd}});
  Rhi::gpu::wait_idle();
  return buffer;
}

// TODO
// template<Buffer BufferType>
// class BufferSpan {
// public:
//   using buffer_type = BufferType;
//   using size_type   = buffer_type::size_type;
//
//   BufferSpan() = default;
//
//   BufferSpan(buffer_type& buffer, size_type offset = 0, size_type size = 0) :
//     buffer(std::addressof(buffer)), span_offset(offset), span_size(size) {
//     assert(offset + size <= buffer.size() && "Buffer span range exceeds buffer size");
//   }
//
//   [[nodiscard]] buffer_type& data() const { return *buffer; }
//
//   [[nodiscard]] size_type offset() const { return span_offset; }
//
//   [[nodiscard]] size_type size() const { return span_size; }
//
// private:
//   buffer_type* buffer;
//   size_type span_offset;
//   size_type span_size;
// };

} // namespace rf3d::rhi
