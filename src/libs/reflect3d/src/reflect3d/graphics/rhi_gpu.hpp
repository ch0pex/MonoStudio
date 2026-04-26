/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rhi_gpu.hpp
 * @date 13/04/2026
 * @brief Render Hardware Interface (RHI) GPU class definition and related utilities
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/rhi_types.hpp"

// --- Dependencies ---

// --- External dependencies ---

// --- STD ---

// --- System ---

namespace rf3d::rhi {

template<RenderHardwareInterface Rhi = impl::DefaultBackend>
submit_info_t<Rhi> default_submit_info(frame_context_t<Rhi>& ctx, surface_t<Rhi> const& surface) {
  return {
    .command_buffers   = {ctx.command_list},
    .wait_semaphores   = {surface.present_semaphore()},
    .signal_semaphores = {surface.render_semaphore()},
    .wait_stages       = {PsoStage::color_attachment_output},
    .signal_fence      = ctx.fence,
  };
}

} // namespace rf3d::rhi
