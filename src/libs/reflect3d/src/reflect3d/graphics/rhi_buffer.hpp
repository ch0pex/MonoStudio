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

// template<DedicatedBuffer Buffer, RenderHardwareInterface Rhi = impl::DefaultBackend>
// Buffer make_buffer(std::ranges::contiguous_range auto initial_data) {
//   Buffer buffer {};
//   transfer_buffer_t<Rhi> staging_buffer {BufferInfo {.data = initial_data}};
//
//   BufferCopyRegion copy_region {.size = initial_data.size_bytes()};
//   copy_command_list_t<Rhi> copy_command_list {};
//   copy_command_list.copy_buffer(staging_buffer, buffer, copy_region);
//
//   // TODO: submit work
//   Rhi::gpu::submit_work(copy_command_list);
//   Rhi::gpu::wait_idle();
//   return buffer;
// }

} // namespace rf3d::rhi
