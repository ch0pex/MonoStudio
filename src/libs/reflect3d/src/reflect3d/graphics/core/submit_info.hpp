/************************************************************************
 * Copyright (c) 2026 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file submit_info.hpp
 * @date 13/04/2026
 * @brief Submit info struct definition for graphics work submission
 */

#pragma once

// --- Includes ---
#include "reflect3d/graphics/api/command_list.hpp"
#include "reflect3d/graphics/api/fence.hpp"
#include "reflect3d/graphics/api/semaphore.hpp"

// --- Dependencies ---
#include <optional>
#include <vector>

// --- External dependencies ---

// --- STD ---

// --- System ---

namespace rf3d {

// TODO: Change vector by mono::span when braced initializer lists have static storage duration (C++26)
// GCC already supports this
template<GraphicsCommandList CmdListType, Semaphore SemaphoreType, Fence FenceType>
struct SubmitInfo {
  std::vector<std::reference_wrapper<CmdListType const>> command_buffers {};
  std::vector<std::reference_wrapper<SemaphoreType const>> wait_semaphores {};
  std::vector<std::reference_wrapper<SemaphoreType const>> signal_semaphores {};
  std::vector<PsoStage> wait_stages {};
  std::optional<std::reference_wrapper<FenceType const>> signal_fence {};
};

} // namespace rf3d
